import java.text.SimpleDateFormat
import groovy.transform.Field
import groovy.json.JsonOutput

@Field static boolean learning = false
@Field static boolean eventFromApp = false
@Field static boolean installed = false


definition(
    name: "A.I. Thermostat Set Point Manager",
    namespace: "elfege",
    author: "ELFEGE",

    description: """

This A.I. will learn from your habits based on the following variables:
- outside temperature
- humidity level (inside or outside, depending on which sensor you use)

From there it will remember which values were most comfortable for you. 
Then it will adjust either a virtual dimmer that will serve as a global variable
or, if you are using it under Hubitat (highly recommended), a connector so your 
other apps can adjust their thermostats' set points accordingly. 

You may expect to spend several weeks setting your thermostats to your liking until the app fully graps the variety of your specific needs

It gets even smarter after an extended period of time (providing you don't reset its database by, for example, uninstalling/reinstalling the app)

""",

    category: "Green Living",
    iconUrl: "https://www.philonyc.com/assets/penrose.jpg",
    iconX2Url: "https://www.philonyc.com/assets/penrose.jpg",
    iconX3Url: "https://www.philonyc.com/assets/penrose.jpg", 
    image: "https://www.philonyc.com/assets/penrose.jpg"
)

preferences {
    page name: "pageConfig"
}
def pageConfig() {

    if(state.paused)
    {
        log.debug "new app label: ${app.label}"
        while(app.label.contains(" (Paused) "))
        {
            app.updateLabel(app.label.minus("(Paused)" ))
        }
        app.updateLabel(app.label + ("<font color = 'red'> (Paused) </font>" ))
    }
    else if(app.label.contains("(Paused)"))
    {
        app.updateLabel(app.label.minus("<font color = 'red'> (Paused) </font>" ))
        while(app.label.contains(" (Paused) "))       
        {
            app.updateLabel(app.label.minus("(Paused)" ))
        }
        log.debug "new app label: ${app.label}"
    }


    def pageProperties = [
        name:       "pageConfig",
        title:      "$app.label",
        nextPage:   null,
        install: true,
        uninstall: true,
        submitOnChange:true
    ]

    dynamicPage(pageProperties) {

        if(state.paused == true)
        {
            state.button_name = "resume"
            logging("button name is: $state.button_name")
        }
        else 
        {
            state.button_name = "pause"
            logging("button name is: $state.button_name")
        }


        section("")
        {
            input "pause", "button", title: "$state.button_name"
        }
        section("Home Location Modes Management")
        {
            input "restricted", "mode", title: "Do not run this app when in these modes:", description:"select modes", multiple:true, required:false
            input "switchrestriction", "capability.switch", title: "Do not run this app when those switches are off", multiple: true, required: false
            input "baseMode", "mode", title: "Select all the modes you want this app to learn with (so it learns from different situations)", multiple: true, required: true, submitOnChange:true
            if(/*state.db.size() != baseMode.size() && */state.installed == true)
            {
                input "checkConsistency", "button", title:"Check Modes Data Consistency", submitOnChange:true
            }
        }
        section([title:"Options"]) {
            label title:"Assign a name", required:false
        }

        section("Select your input devices")
        { 
            input(name: "Thermostat", type: "capability.thermostat", title: "select a thermostat", required: true, multiple: false, description: null, submitOnChange:true)
            input(name: "outdoor", type: "capability.temperatureMeasurement", title: "select an outdoor temperature measurement device", required: true, multiple: false, description: null, submitOnChange:true)
            input(name: "humidity", type: "capability.relativeHumidityMeasurement", title: "select a humidity measurement device", required: true, multiple: false, description: null, submitOnChange:true)
        }
        section("select your connector")
        {
            input(name: "option", type: "enum", title: "select a connector type", required: true, submitOnChange: true, options: ["HE connector", "dimmer"])
            if(option == "dimmer")
            {
                input(name: "dimmer", type: "capability.switchLevel", title: "Select a dimmer", required:true, submitOnChange:true)
            }
            else
            {
                paragraph "this option is not yet implemented, sorry! look for future updates!"
            }
        }
        section("Optional: Set your own base line from which the app will learn")
        {
            if(state.installed)
            {
                input "setbaseline", "bool", title: "Set a new base line", submitOnChange:true, defaultValue:false
                if(setbaseline)
                {
                    paragraph "<h3>This will overwrite your current database but it will keep what this app has learned since fist installation (you won't lose data)</h3>"
                    input "baseline", "number", title: "enter a value between 1 and 108", range: "1..108"
                    //input "valuestosave", "text", title: "Write down the temp values you don't want to delete", description: "separate each value with a comma", submitOnChange:true
                    input "baseLineModes", "enum", title: "Set this baseline only for the following modes", options: baseMode.sort(), multiple:true, submitOnChange:true

                    input "baseconfirm", "button", title: "confirm"
                }
            }

        }
        section()
        {
            if(state.installed)
            {
                input "update", "button", title: "UPDATE"
                input "testdataretrieve", "button", title: "TEST DATA RETRIEVAL"

                if(state.confirm)
                {
                    def message = "CAREFULL! THIS WILL OVERWRITE ALL PREVIOUSLY LEARNED INPUTS!"
                    descriptiontext message
                    paragraph message
                    input "confirm", "button", title:"confirm deletion"
                    input "cancel", "button", title:"cancel deletion"
                }
                paragraph "                                        ", width: 6
                input "resetdb", "button", title: "RESET", width: 6
                paragraph "                                        ", width: 6
            }
            input "enablelogging", "bool", title:"Debug", defaultValue:false, submitOnChange:true
            input "enabletext", "bool", title: "Description Text", defaultValue:true, submitOnChange:true
            if(enablelogging)
            {   
                state.EnableDebugTime = now()
            }
        }
    }
}
def installed() {
    logging("Installed with settings: ${settings}")

    initialize()
    createDb()
    state.installed = true
    state.valuestosave = []
    installed = true

}
def updated() {
    state.installed = true


    logging("""updated with settings: ${settings}-""")
    state.label = app.label
    unschedule()
    initialize()
}
def initialize(){

    log.info "initializing...${app.label}..."

    if(!state.EnableDebugTime){ 
        state.EnableDebugTime = now() 
    }


    if(Thermostat.currentValue("thermostatMode") == "heat")
    {
        state.lastMode = "heatingSetpoint"
    }
    else if (Thermostat.currentValue("thermostatMode") == "cool")
    {
        state.lastMode = "coolingSetpoint"
    }
    else 
    {
        state.lastMode = "heatingSetpoint"
    }

    learning = false
    state.lastEvent = 60000 


    unschedule()
    unsubscribe()
    switchrestriction
    subscribe(switchrestriction, "heatingSetpoint", switchHandler)
    subscribe(Thermostat, "heatingSetpoint", setPointHandler)
    subscribe(Thermostat, "coolingSetpoint", setPointHandler)
    //subscribe(Thermostat, "thermostatSetpoint", setPointHandler)

    subscribe(dimmer, "level", setPointHandler) 
    subscribe(outdoor, "temperature", temperatureHandler)
    subscribe(humidity,  "humidity", humidityHandler)

    if(state.installed && installed)
    {
        createDb() // for development only, comment out if not developing app or db will be reset after every update
    }

    if(state.installed)
    {
        checkConsistency()
    }

    descriptiontext "Initialization complete!"

}
def switchHandler(evt){
    descriptiontext "$evt.device is $evt.value"
}
def appButtonHandler(btn) {
    switch(btn) {
        case "pause":state.paused = !state.paused
        log.debug "state.paused = $state.paused"
        if(state.paused)
        {
            log.debug "unscheduling all tasks..."
            unschedule()
            log.debug "unsuscribing from events..."
            unsubscribe()         
            break
        }
        else
        {
            updated()            
            break
        }
        case "update":
        state.paused = false
        updated()
        break
        case "resetdb":state.confirm = true
        break
        case "confirm":state.confirm = false
        state.db = [:] // delete the old db
        runIn(1, createDb)
        break
        case "cancel":state.confirm = false
        break
        case "baseconfirm":
        if(baseline) {setBaseLine()}
        break
        case "showdata":
        state.showdata = true
        runIn(30, cancelshowdata)
        case "testdataretrieve":
        retrieve()
        break
        case "checkConsistency":
        checkConsistency()
        break

    }
}
def setPointHandler(evt){

    if(!state.paused )
    {       
        descriptiontext "$evt.device $evt.name is now $evt.value eventFromApp = $eventFromApp elapsed time since last event: ${(now() - state.lastEvent)/1000} seconds"
        if(state.EnableDebugTime == "null" || state.EnableDebugTime == null){state.EnableDebugTime = now() }

        if((now() - state.EnableDebugTime) > 30*60*1000)
        {
            disablelogging()
        }

        def ignoreMessage = ""
        boolean swOn = switchrestriction?.find{it.currentValue("switch") == "on"}
        boolean restrictedMode = location.mode in restricted

        if(restrictedMode)
        {
            ignoreMessage = "restricted mode"
        }
        if(Thermostat.currentValue("thermostatMode") == "auto")

        {
            ignoreMessage += "Not learning, override due to 'auto' mode"
        }

        if(switchrestriction && !swOn) {
            descriptiontext("not learning because $switchrestriction are all off")
        }
        if(now() - state.lastEvent < 60000) {
            ignoreMessage += """
2 last events too close!"""
            state.lastEvent = now() 
        }
        if(learning) {
            ignoreMessage += """
App already busy learning")"""
            state.lastEvent = now() 
        }
        if(eventFromApp) {
            ignoreMessage += """
This event was triggered by this app"""
            state.lastEvent = now() 
        }

        state.lastEvent = now() 
        if(ignoreMessage != ""){
            log.warn ignoreMessage            
            return
        }
        else
        {
            learning = true
            //// now learn from this new input
            learn(evt.value) 
        }
    }
    eventFromApp = false
}
def temperatureHandler(evt){ 
    boolean restrictedMode = location.mode in restricted

    logging("$evt.device returns ${evt.name}:${evt.value}")
    if(!state.paused && !restrictedMode)
    {
        logging("retrieving recording values now")
        retrieve()
    }
}
def humidityHandler(evt){
    boolean restrictedMode = location.mode in restricted
    logging("$evt.device returns ${evt.name}:${evt.value}")
    if(!state.paused && !restrictedMode)
    {
        logging("retrieving recording values now")
        retrieve()
    }
}
def retrieve(){

    if(!learning)
    {
        checkConsistency() // check consistency

        def outsideTemp = outdoor.currentValue("temperature")
        def humidity = humidity.currentValue("humidity")
        boolean found = false
        // we want to find the item in db that contains this evt.value outside temp and current humidity so we can extrat index 2, that is the corresponding desired temp
        def ref = [outsideTemp, humidity] // values to look for
        def desired = null
        def a = 0
        def sa = state.db.size() 

        for(sa!=0;a<sa&&!found;a++)
        {      
            log.trace "Reading database for ${baseMode[a]} mode..."
            def dbX = state.db.find{it.key == baseMode[a]}?.value
            def i = 0
            def s = dbX.size()
            if(baseMode[a] == location.mode)
            {
                for(s!=0;i<s&&!found;i++)
                {

                    if(dbX[i][0,1] == ref[0,1])
                    {
                        found = true
                        descriptiontext """
FOUND A MATCH at index $i for outside temperature: $outsideTemp in ${baseMode[a]}'s data set 
reference: ${ref[0,1]} matches:${dbX[i][0,1,2]} New value: ${dbX[i][2]}
current mode = $location.mode
modified data mode = ${baseMode[a]} (shoud be identical to current location mode)
"""
                        desired = dbX[i][2]
                        break;
                    }
                }
            }
            else 
            {
                descriptiontext "location not in ${baseMode[a]} mode"
            }
        }
        if(!found)
        {
            log.warn "[outsideTemp, humidity] : [$outsideTemp, $humidity] found NO MATCH! PLEASE FIX DATABASE..."
            return
        }

        logging("new desired temperature for current weather is: ${desired}??F")
        if(dimmer.currentValue("level") != desired)
        {
            eventFromApp = true
            dimmer.setLevel(desired.toInteger())
            descriptiontext("$dimmer set to $desired")
        }
        else 
        {
            logging("$dimmer is already set to $desired")
        }
    }
    else 
    {
        log.warn "APP IS CURRENTLY LEARNING, IGNORING REQUEST TO PREVENT DATA OVERFLOW"
    }
}
def setBaseLine(){
    if(!state.valuestosave)
    {
        state.valuestosave = []
    }

    checkConsistency() // check consistency

    descriptiontext "processing the data. Please, be patient, this can take several minutes and may temporarily slow down your hub"
    if(state.db == null)
    {
        log.warn "DATA BASE NOT YET CREATED. PLEASE FINISH INSTALLING THIS APP BEFORE HITTING THAT BUTTON"
        return
    }

    def a = 0
    def sa = state.db.size() 

    descriptiontext """
values to save: $state.valuestosave
state.db size = $sa
a<s = ${a<s}
sa!=0 = ${sa!=0}
"""

    long start = now()
    long counter = now()
    long timer = 30000


    for(sa!=0;a<sa;a++)
    {      
        def thisMode = baseMode[a]
        if(thisMode in baseLineModes)
        {
            log.trace "Writting database for ${baseMode[a]} mode..."
            def dbX = state.db.find{it.key == baseMode[a]}?.value
            def i = 0
            def s = dbX.size()

            //return 

            for(s!=0;i<s;i++)
            {   
                def ref = dbX[i][2] 
                def dataFound = state.valuestosave.find{it == ref}
                boolean skip = ref in dataFound

                if(skip)
                {
                    log.warn "value not replaced at user's request because ${ref} was found in ${state.valuestosave}"
                }
                else 
                {
                    //descriptiontext(".")
                    dbX[i].remove(2)
                    dbX[i] += baseline
                }
                if(now() - start >= 500)
                {
                    log.warn "Updating database... currently at index #$i within $a"
                    start = now()
                }
                if(now() - counter >= timer)
                {
                    log.warn "DATA UPDATE TIME OUT!"
                    return
                }
                state.db."${baseMode[a]}" = dbX
            }
        }
        else
        {
            descriptiontext "${baseMode[a]} is not the mode user asked for"
        }
    }
    log.trace "BASE LINE SUCCESSFULY UPDATED!"
}
def createDb(){
    descriptiontext "GENERATING NEW DATABASE... this may take some ressources for a little while..."

    int ms = baseMode.size()
    int m = 0
    state.db = [:] // one DB per mode

    for(ms != 0; m < ms; m++)
    {

        int h = 10 // minimum humidity value
        int sh = 100-h // humidity goes from 10 to 100%
        int i = -50 // minimum outside temp // If you think it can get below that... move to Venus...
        int s = 120 + Math.abs(i) // max outside temp expected, beyond that... move to Mars... 

        def db = []
        def insert = []
        long start = now()
        long timer = 30000
        def bsln = baseline
        if(!baseline)
        {
            bsln = 75
        }

        for(s!=0;i<s;i++)
        {
            int a = 0
            h = 10
            for(sh!=0;a<s;a++) // all variations of humidity for every i (i being outside temp)
            { // s is max size = 120+abs(i) don't put <sh here
                if(h<100){h += 1}
                insert = [i,h,bsln] // create [[outside, humidity, setpoint]]
                db << insert
                if(now() - start > timer)
                {
                    log.warn "createDb() (inner loop) took more than $timer seconds to execute, BREAK"
                    return
                }
            }
            if(now() - start > timer)
            {
                log.warn "createDb() (outer loop) took more than $timer seconds to execute, BREAK"
                return
            }
        }

        def newIt = ["${baseMode[m]}" : db]
        state.db << newIt
        log.debug "DB for ${baseMode[m]} mode done...  "
    }

    //metaDB = ["Day":["test", "test", "test"], "Night":["test", "test", "test"], "Evening":["test", "test", "test"]]
    //state.metaDB = metaDB

    log.debug "ALL DATA SUCCESSFULY GENERATED"

}
def checkConsistency(){
    log.trace "VERIFYING DATABASE CONSISTENCY..."

    def x = baseMode.size()
    def dbX = state.db.find{it.key == baseMode[0]}?.value // take the first key's value as baseline

    descriptiontext """

ALL location modes are : $location.modes
Modes managed by this app are: $baseMode

"""
    logging"""
state.db.size() < baseMode.size() ? ${state.db?.size() < baseMode?.size()}
state.db.size() > baseMode.size() ? ${state.db?.size() > baseMode?.size()}
"""


    if(state.db.size() < baseMode.size())
    {
        log.warn "Main database is too small, updating now"
        def s = baseMode.size()
        def i = 0
        for(s!=0;i<s;i++)
        {
            def inDb = state.db.find{it.key == baseMode[i]}?.key // if null, then it's a missing value that needs to be implemented into the db
            //log.debug "inDb = ${inDb}"
            boolean missing = inDb == null
            //return
            if(missing)
            {
                descriptiontext "updating database with ${baseMode[i]} mode as a new key"
                state.db."${baseMode[i]}" = dbX
            }
            else
            {
                descriptiontext "$inDb is an existing key"
            }  
        }
    }
    else if(state.db.size() > baseMode.size())
    {
        log.warn "Some modes are no longer in use, updating now"
        def s = location.modes.size()
        def i = 0
        for(s!=0;i<s;i++)
        {
            // if one of the hub's location modes is found in state.db but NOT in baseMode, then it needs to be deleted from state.db
            boolean inDb = state.db.find{it.key == "${location.modes[i]}"}?.key
            if(inDb) {log.debug "${location.modes[i]} is a key"}
            boolean inBM = baseMode.find{it == "${location.modes[i]}"}
            boolean noLongerInBaseMode = !inBM && inDb
            log.warn "noLongerInBaseMode = $noLongerInBaseMode for (${location.modes[i]})"

            if(noLongerInBaseMode)
            {
                descriptiontext "deleteding ${location.modes[i]} from database"
                def toremove = "${location.modes[i]}" // for some reason "state.db.remove(location.modes[i])" (even with brackets in string mode) doesn't remove the entry... "
                state.db.remove(toremove.toString()) 
            } 
        }

    }
    else 
    {
        descriptiontext "Database sizes are consistent"
    }
}
def learn(newSP){


    checkConsistency()
    if(!state.valuestosave) {state.valuestosave = []}
    log.trace "NOW LEARNING"
    state.lastLearnFailed = true // if this isn't reset to false after execution of this function, then it means that the database needs fixing
    // save learned values as a backup to allow for future safe rewrite/repair by setBaseLine()
    boolean TempValAlreadyThere = state.valuestosave.find{it == newSP}
    if(!TempValAlreadyThere) // don't allow this db to get too big
    {
        descriptiontext "$newSP added to the list of values to save for future database repairs"
        state.valuestosave << newSP 
    }
    else
    {
        logging "$newSP already recorded as a learning value in the past"
    }

    def outsideTemp = outdoor.currentValue("temperature")
    def humidity = humidity.currentValue("humidity")
    def ref = [outsideTemp, humidity, newSP]
    long safetyDelay = 60000
    long start = now() 
    long counter = now()
    learning = false
    def a = 0
    def sa = state.db.size() 
    def backup = state.db

    descriptiontext( " ****** Learning new desired temperature: $newSP with outside temp: $outsideTemp and humidity: $humidity")

    for(sa!=0;a<sa;a++)
    {      
        learning = true

        def dbX = state.db.find{it.key == baseMode[a]}?.value
        if(baseMode[a] == location.mode)
        {
            log.trace "Updating database for ${baseMode[a]} mode with ${dbX.size()} entries to check"

            int amp = 8 // amplitude


            def indices = [0, 1]
            def index = null
            int n = 0 // 
            int f = 8 // amplitude of temperatures and humidity levels for which this app will remember the new setpoint
            /// outsideTemp amplitude loop

            for(f!=0;n<f;n++)
            {          
                def data1 = dbX.find{it[indices] == [outsideTemp + n, humidity]}; if(data1 && data1[2] != newSP){ index = dbX.indexOf(data1);dbdg(data1); dbX[index] = [outsideTemp + n, humidity, newSP]}
                def data2 = dbX.find{it[indices] == [outsideTemp - n, humidity]}; if(data2 && data2[2] != newSP){ index = dbX.indexOf(data2);dbdg(data2);  dbX[index] = [outsideTemp - n, humidity, newSP]} 
                def data3 = dbX.find{it[indices] == [outsideTemp + n, humidity + n]}; if(data3 && data3[2] != newSP){ index = dbX.indexOf(data3);dbdg(data3);  dbX[index] = [outsideTemp + n, humidity - n, newSP]}
                def data4 = dbX.find{it[indices] == [outsideTemp - n, humidity - n]}; if(data4 && data4[2] != newSP){ index = dbX.indexOf(data4);dbdg(data4);  dbX[index] = [outsideTemp + n, humidity - n, newSP]}
                def data5 = dbX.find{it[indices] == [outsideTemp - n, humidity + n]}; if(data3 && data3[2] != newSP){ index = dbX.indexOf(data3);dbdg(data5);  dbX[index] = [outsideTemp + n, humidity - n, newSP]}
                def data6 = dbX.find{it[indices] == [outsideTemp + n, humidity - n]}; if(data4 && data4[2] != newSP){ index = dbX.indexOf(data4);dbdg(data6);  dbX[index] = [outsideTemp + n, humidity - n, newSP]}           
                def data7 = dbX.find{it[indices] == [outsideTemp, humidity - n]}; if(data7 && data7[2] != newSP){ index = dbX.indexOf(data7);dbdg(data7);  dbX[index] = [outsideTemp, humidity - n, newSP]}           
                def data8 = dbX.find{it[indices] == [outsideTemp, humidity + n]}; if(data8 && data8[2] != newSP){ index = dbX.indexOf(data8);dbdg(data8);  dbX[index] = [outsideTemp, humidity + n, newSP]}           

                if(now() - counter > 10000)
                {
                    log.warn "Processing, please be patient... index #$n out of $f elapsed time is: ${(now() - start)/1000} seconds"
                    counter = now()
                }
                if(now() - start > safetyDelay)
                {
                    log.error "TIMEOUT! learn() took more than ${(safetyDelay/1000)} seconds to execute. Stopping operation and restoring backup (inner loop)"
                    state.db = backup
                    return
                }
            }

            state.db."${baseMode[a]}" = dbX
        }
        else {
            descriptiontext "location not in ${baseMode[a]} mode"
        }
    }
    state.lastLearnFailed = false
    learning = false

    descriptiontext "LEARNING SUCCESSFUL! The operation took ${(now() - start)/1000} seconds)"
}
def dbdg(data)
{
    log.debug "updating $data"   
}
def logging(message){
    //state.enablelogging = true
    if(state.enablelogging) log.debug message

}
def descriptiontext(message){
    //state.enablelogging = true
    if(enabletext) log.info message

}
def disablelogging(){
    app.updateSetting("enablelogging",[value:"false",type:"bool"])
    log.warn "debug logging disabled!" 
}
