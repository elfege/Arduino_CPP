import java.text.SimpleDateFormat
import groovy.transform.Field
import groovy.json.JsonOutput

@Field static boolean learning = false
@Field static boolean eventFromApp = false



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

    /*if(state.paused)
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
}*/


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
            input "baseMode", "mode", title: "Select all the modes you want this app to learn with (so it learns from different situations)", multiple: true, required: true
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
            input "setbaseline", "bool", title: "Set a new base line", submitOnChange:true, defaultValue:false
            if(setbaseline)
            {
                paragraph "<h3>This will overwrite your current database. You may want to save some values (see below)</h3>"
                input "baseline", "number", title: "enter a value between 1 and 108", range: "1..108"
                input "valuestosave", "text", title: "Write down the temp values you don't want to delete", description: "separate each value with a comma", submitOnChange:true

                input "baseconfirm", "button", title: "confirm"
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
                    log.info message
                    paragraph message
                    input "confirm", "button", title:"confirm deletion"
                    input "cancel", "button", title:"cancel deletion"
                }
                paragraph "                                        ", width: 6
                input "resetdb", "button", title: "RESET", width: 6
                paragraph "                                        ", width: 6
            }
            input "enablelogging", "bool", title:"Debug", defaultValue:false, submitOnChange:true

        }

        section("")
        {
            input "showdata", "bool", title: "Show the current data", submitOnChange:true

            def db = state.db

            if(db != null && showdata)
            {

                def MybaseMode = []
                MybaseMode = baseMode.sort()
                //baseMode.each {MybaseMode << "$it"}
                input "dbNumber", "enum", options: baseMode.sort(), title: "Select which home location mode Database you want to see", submitOnChange:true
                def selected = db.find{it.key == dbNumber}?.value
                log.info """
db = $db
selected = $selected
"""
                if(dbNumber && db.size() > 1 && selected)
                {
                    def truncatedSelection = selected[0,1,2]
                    paragraph "Here is a sample of your database: ${truncatedSelection}"
                }
                else if(db.size() < 1)
                {
                    paragraph "No value yet. Finish installing or reset database first"
                }
                else if(dbNumber)
                {
                    paragraph "Something went wrong... contact developper"
                }
            }
        }
    }
}

def installed() {
    logging("Installed with settings: ${settings}")
    state.installed = true

    initialize()
    createDb()
}
def updated() {
    if(!state.installed)
    {
        installed()
    }
    if(enablelogging)
    {
        state.enablelogging = true
        runIn(1800, disablelogging)
        log.info "logging enabled"
    }
    else 
    {
        state.enablelogging = false
        log.warn "logging disabled"
    }

    logging("""updated with settings: ${settings}-""")
    state.label = app.label
    unschedule()
    initialize()
}
def initialize(){
    logging("initializing ${app.label}...")

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
    state.lastEvent = now() 


    unschedule()
    unsubscribe()

    subscribe(Thermostat, "heatingSetpoint", setPointHandler)
    subscribe(Thermostat, "coolingSetpoint", setPointHandler)
    subscribe(Thermostat, "thermostatSetpoint", setPointHandler)

    subscribe(dimmer, "level", setPointHandler) 
    subscribe(outdoor, "temperature", temperatureHandler)
    subscribe(humidity,  "humidity", humidityHandler)

    //createDb() // for development only, comment out if not developing app or db will be reset after every update

    log.info "Initialization complete!"

}
def initdbSave(){
    state.valuestosave = []

    if(valuestosave)
    { 
        def list = valuestosave.split(',').collect{it as int}
        log.warn "values to save are ${list} and size is ${list.size()}"
        state.valuestosave = list
    }
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
        initdbSave()
        log.info "Running baseline function"
        if(baseline) {setBaseLine()}
        break
        case "showdata":
        state.showdata = true
        runIn(30, cancelshowdata)
        case "testdataretrieve":
        retrieve()
        break

    }
}

def setPointHandler(evt){


    boolean restrictedMode = location.mode in restricted

    if(!restrictedMode)
    {
        if(eventFromApp || (evt.device == Thermostat && dimmer.currentValue("level") == evt.value))
        {
            log.warn "Event generated by this app, not learning from this"
            eventFromApp = false
            return
        }

        log.info "$evt.name is now $evt.value"
        def thermMode = Thermostat.currentValue("thermostatMode")
        if(state.lastMode != evt.name && evt.name != "level")
        {

            log.info "ignoring this event state.lastMode = $state.lastMode && thermMode = $thermMode" // preventing coolingsetpoint and heatingsepoint from mixing each other
            return 
        }
        if(thermMode == "heat")
        {
            state.lastMode = "heatingSetpoint"
        }
        else if(thermMode == "cool")
        {
            state.lastMode = "coolingSetpoint"
        }
        else 
        {
            state.lastMode = "heatingSetpoint"
        }
        if(now() - state.lastEvent < 5000)
        {
            log.info "events too close!"
            return
        }


        if(!learning)
        {
            if(!state.paused)
            {
                if(Thermostat.currentValue("thermostatMode") != "auto")
                {
                    learning = true
                    state.evtVal = evt.value
                    pauseExecution(1000)
                    runIn(3, learn) // "time" for state to be written into HE's DB
                }
                else 
                {
                    log.info "Not learning, override due to 'auto' mode"
                }
            }
            else 
            {
                log.info "app paused"
            }
        }
        else 
        {
            log.warn "****************** $app.label already learning, skipping this request ******************"
        }
    }
    else 
    {
        log.info "restricted mode"
    }
    state.lastEvent = now() 
}
def learn(){
    def outsideTemp = outdoor.currentValue("temperature")
    def humidity = humidity.currentValue("humidity")
    def newSP = state.evtVal

    ref = [outsideTemp, humidity, newSP]
  
   


    long safetyDelay = 300000
    long start = now() 
    long counter = now()

    learning = false

    def a = 0
    def sa = state.db.size() 
     log.info( " ****** Learning new desired temperature: $newSP with outside temp: $outsideTemp and humidity: $humidity")

    for(sa!=0;a<sa;a++)
    {      
        
        def dbX = state.db.find{it.key == baseMode[a]}?.value
        def i = 0
        def s = dbX.size()
        log.trace "Updating database for ${baseMode[a]} mode with ${dbX.size()} entries to check"

        for(s!=0;i<s;i++)
        {    
            learning = true

            //log.info("$i")
            int n = 1 // we start by adding +1 not +0
            int f = 8 // amplitude

            for(f!=0;n<f;n++)
            {                            
                boolean A = dbX[i][0] == outsideTemp && dbX[i][1] == humidity // this will apply the new setpoint value to the current outsideTemp and humidity values

                // now we test for surrounding values within an amplitude of +/- n for both outsideTemp and humidity
                boolean B = dbX[i][0] == outsideTemp && dbX[i][1] == humidity + n
                boolean C = dbX[i][0] == outsideTemp && dbX[i][1] == humidity - n
                boolean D = dbX[i][0] == outsideTemp + n && dbX[i][1] == humidity + n
                boolean E = dbX[i][0] == outsideTemp - n && dbX[i][1] == humidity - n
                boolean F = dbX[i][0] == outsideTemp + n && dbX[i][1] == humidity
                boolean G = dbX[i][0] == outsideTemp - n && dbX[i][1] == humidity
                boolean H = dbX[i][0] == outsideTemp + n && dbX[i][1] == humidity - n
                boolean IA = dbX[i][0] == outsideTemp - n && dbX[i][1] == humidity + n

                if(A||B||C||D||E||F||G||H||IA)
                {
                    dbX[i].remove(2)
                    dbX[i] += newSP 
                }
            }
            
            
            if(now() - start > safetyDelay)
            {
                log.warn "setPointHandler() took more than $safetyDelay seconds to execute, BREAK"
                return
            }
            if(now() - counter > 10000)
            {
                log.warn "Still processing, please be patient... index #$i out of $s"
                counter = now()
            }
            
            state.db."${baseMode[a]}" = dbX
        }
    }
    learning = false

    log.info "DONE LEARNING. The operation took ${(now() - start)/1000} seconds (${(now() - start)/1000/60} minutes)"
    //updated() // re-subscribe to events 

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
            log.trace "Writting database for ${baseMode[a]} mode..."
            def dbX = state.db.find{it.key == baseMode[a]}?.value
            def i = 0
            def s = dbX.size()

            for(s!=0;i<s&&!found;i++)
            {

                if(dbX[i][0,1] == ref[0,1])
                {
                    found = true
                    log.info """
FOUND A MATCH at index $i for outside temperature: $outsideTemp in ${baseMode[a]}'s data set 
reference: ${ref[0,1]} matches:${dbX[i][0,1,2]} New value: ${dbX[i][2]}
"""
                    desired = dbX[i][2]
                    break;
                }
            }
        }
        if(!found)
        {
            log.warn "[outsideTemp, humidity] : [$outsideTemp, $humidity] found NO MATCH! PLEASE FIX DATABASE..."
            return
        }

        logging("new desired temperature for current weather is: ${desired}°F")
        if(dimmer.currentValue("level") != desired)
        {
            eventFromApp = true
            dimmer.setLevel(desired.toInteger())
            log.info("$dimmer set to $desired")
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
    log.info "processing the data. Please, be patient, this can take several minutes and may temporarily slow down your hub"
    if(state.db == null)
    {
        log.warn "DATA BASE NOT YET CREATED. PLEASE FINISH INSTALLING THIS APP BEFORE HITTING THAT BUTTON"
        return
    }

    def a = 0
    def sa = state.db.size() 
    def toSave = state.valuestosave

    log.info """
values to save: $toSave
state.db size = $sa
a<s = ${a<s}
sa!=0 = ${sa!=0}
"""

    long start = now()
    long counter = now()
    long timer = 120000


    for(sa!=0;a<sa;a++)
    {      
        log.trace "Writting database for ${baseMode[a]} mode..."
        def dbX = state.db.find{it.key == baseMode[a]}?.value
        def i = 0
        def s = dbX.size()

        //return 

        for(s!=0;i<s;i++)
        {   
            def ref = dbX[i][2] 
            def dataFound = toSave.findAll{it == ref.toInteger()}
            boolean skip = ref.toInteger() in dataFound
            if(skip)
            {
                log.warn "value not replaced at user's request: ${ref} was found in ${toSave}" 
            }
            else 
            {
                //log.info(".")
                dbX[i].remove(2)
                dbX[i] += baseline
            }
            if(now() - start >= 500)
            {
                log.warn "Updating database... currently at index #$i within $ii"
                start = now()
            }
            if(now() - counter >= timer)
            {
                log.warn "DATA UPDATE FAILED because it took too long to run"
                return
            }
            state.db."${baseMode[a]}" = dbX
        }
    }
    log.trace "BASE LINE SUCCESSFULY UPDATED!"
}
def createDb(){
    log.info "GENERATING NEW DATABASE... this may take some ressources for a little while..."

    int ms = baseMode.size()
    int m = 0
    state.db = [:] // one DB per mode

    for(ms != 0; m < ms; m++)
    {

        int h = 10 // minimum humidity value
        int sh = 100-h // humidity goes from 10 to 100%
        int i = -50 // minimum outside temp
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

def logging(message)
{
    //state.enablelogging = true
    if(state.enablelogging) log.debug message

}
def disablelogging()
{
    log.warn "debug logging disabled!" 
    state.enablelogging = false
}
