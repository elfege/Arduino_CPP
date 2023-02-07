definition(
    name: "A.I. Thermostat Set Point Manager",
    namespace: "elfege",
    author: "WIFI_SSID",

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
        //app.updateLabel(app.label + ("<font color = 'red'> (Paused) </font>" ))

        log.debug "new app label: ${app.label}"

        while(app.label.contains(" (Paused) "))
        {
            app.updateLabel(app.label.minus("(Paused)" ))
        }
        //app.updateLabel(app.label.minus("<font color = 'red'> (Paused) </font>" ))
        app.updateLabel(app.label + ("<font color = 'red'> (Paused) </font>" ))

    }
    else if(app.label.contains("(Paused)"))
    {
        app.updateLabel(app.label.minus("<font color = 'red'> (Paused) </font>" ))
        //app.updateLabel(app.label.minus(" "))
        while(app.label.contains(" (Paused) "))        {app.updateLabel(app.label.minus("(Paused)" ))}

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
            log.debug "button name is: $state.button_name"
        }
        else 
        {
            state.button_name = "pause"
            log.debug "button name is: $state.button_name"
        }


        section("")
        {
            input "pause", "button", title: "$state.button_name"
        }
        section("restricted modes")
        {
            input "restricted", "mode", title: "Do not run this app when in these modes:", description:"select modes", multiple:true, required:false
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
            input "update", "button", title: "UPDATE"
            paragraph " ", width: 6
            input "resetdb", "button", title: "RESET", width: 6
            input "testdataretrieve", "button", title: "TEST DATA RETRIEVAL", width: 6
            if(state.confirm)
            {
                def message = "CAREFULL! THIS WILL OVERWRITE ALL PREVIOUS LEARNED INPUTS!"
                log.info message
                paragraph message
                input "confirm", "button", title:"confirm deletion"
                input "cancel", "button", title:"cancel deletion"
            }
            input "enablelogging", "bool", title:"Debug", defaultValue:false, submitOnChange:true
            if(enablelogging)
            {
                state.enablelogging = true
                runIn(240, disablelogging)
                log.info "logging enabled"
            }
            else 
            {
                state.enablelogging = false
                log.warn "logging disabled"
            }
        }

        section("")
        {
            input "showdata", "bool", title: "Show the current data", submitOnChange:true
            if(showdata)
            {
                paragraph "${state.db}"
            }
        }
    }
}

def installed() {
    logging("Installed with settings: ${settings}")

    initialize()
    createDb()
}
def updated() {

    logging("""updated with settings: ${settings}-""")
    state.label = app.label
    unschedule()
    initialize()
}
def initialize(){
    //app.updateLabel("<font color = 'red'> ${app.label} </font>" )

    logging("initializing ${app.label}...")

    unschedule()
    unsubscribe()



    subscribe(Thermostat, "heatingSetpoint", setPointHandler)
    subscribe(Thermostat, "coolingSetpoint", setPointHandler)
    //subscribe(Thermostat, "thermostatSetpoint", setPointHandler)

    //subscribe(dimmer, "level", setPointHandler) // feedback loop here, do not uncomment this line for now
    subscribe(outdoor, "temperature", temperatureHandler)
    subscribe(humidity,  "humidity", humidityHandler)

    //state.db = createDb() // for development only, comment out if not developing app or db will be reset on every update

    log.info "Initialization complete!"

}
def initdbSave()
{
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
        createDb()
        break
        case "cancel":state.confirm = false
        break
        case "baseconfirm":
        initdbSave()
        log.info "processing the data. Please, be patient, this can take several minutes and may temporarily slow down your hub"
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
        if(!state.paused)
        {
            if(Thermostat.currentValue("thermostatMode") != "auto")
            {

                def outsideTemp = outdoor.currentValue("temperature")
                def humidity = humidity.currentValue("humidity")

                ref = [outsideTemp, humidity, evt.value]
                i = 0
                s = state.db.size()  // we have 7 db into which we need look for those values (outside temp, humidity), try each one until found

                long safetyDelay = 300000
                long start = now() 
                long counter = now()

                unsubscribe() // instead we use this, which is the only way to prevent overlap requests

                log.info( " ****** db size = $s entries. Learning new desired temperature: $evt.value with outside temp: $outsideTemp and humidity: $humidity")

                for(s!=0;i<s;i++)
                {    
                    //log.info("$i")
                    int n = 1 // we start by adding +1 not +0
                    int f = 8 // amplitude

                    for(f!=0;n<f;n++)
                    {                            
                        boolean A = state.db[i][0] == outsideTemp && state.db[i][1] == humidity // this will apply the new setpoint value to the current outsideTemp and humidity values

                        // now we test for surrounding values within an amplitude of +/- n for both outsideTemp and humidity
                        boolean B = state.db[i][0] == outsideTemp && state.db[i][1] == humidity + n
                        boolean C = state.db[i][0] == outsideTemp && state.db[i][1] == humidity - n
                        boolean D = state.db[i][0] == outsideTemp + n && state.db[i][1] == humidity + n
                        boolean E = state.db[i][0] == outsideTemp - n && state.db[i][1] == humidity - n
                        boolean F = state.db[i][0] == outsideTemp + n && state.db[i][1] == humidity
                        boolean G = state.db[i][0] == outsideTemp - n && state.db[i][1] == humidity
                        boolean H = state.db[i][0] == outsideTemp + n && state.db[i][1] == humidity - n
                        boolean IA = state.db[i][0] == outsideTemp - n && state.db[i][1] == humidity + n

                        if(A||B||C||D||E||F||G||H||IA)
                        {
                            state.db[i].remove(2)
                            state.db[i] += evt.value 
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
                }
                log.info "DONE LEARNING. The operation took ${(now() - start)/1000} seconds (${(now() - start)/1000/60} minutes)"
                updated() // re-subscribe to events 

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
        log.info "restricted mode"
    }
}

def temperatureHandler(evt)
{ 
    boolean restrictedMode = location.mode in restricted

    logging("$evt.device returns ${evt.name}:${evt.value}")
    if(!state.paused && !restrictedMode)
    {
        logging("retrieving recording values now")
        retrieve()
    }
}
def humidityHandler(evt)
{
    boolean restrictedMode = location.mode in restricted
    logging("$evt.device returns ${evt.name}:${evt.value}")
    if(!state.paused && !restrictedMode)
    {
        logging("retrieving recording values now")
        retrieve()
    }
}
def retrieve()
{
    def outsideTemp = outdoor.currentValue("temperature")
    def humidity = humidity.currentValue("humidity")

    def i = 0
    def s = state.db.size() 
    boolean found = false


    // we want to find the item in db that contains this evt.value outside temp and current humidity so we can extrat index 2, that is the corresponding desired temp
    def ref = [outsideTemp, humidity] // values to look for
    def desired = null

    for(s!=0;i<s&&!found;i++)
    {

        if(state.db[i][0,1] == ref[0,1])
        {
            found = true
            log.info("FOUND A MATCH at index $i for outside temperature: $outsideTemp ||| reference: ${ref[0,1]} matches:${state.db[i][0,1,2]}")
            desired = state.db[i][2]

            break;
        }
    }
    if(!found)
    {
        log.warn "NO MATCH! PLEASE FIX DATABASE..."
        return
    }

    logging("new desired temperature for current weather is: ${desired}°F")
    if(dimmer.currentValue("level") != desired)
    {
        dimmer.setLevel(desired)
        log.info("$dimmer set to $desired")
    }
    else 
    {
        logging("$dimmer is already set to $desired")
    }
}

def setBaseLine()
{
    if(state.db == null)
    {
        log.warn "DATA BASE NOT YET CREATED. PLEASE FINISH INSTALLING THIS APP BEFORE HITTING THAT BUTTON"
        return
    }

    log.trace "DB OVERWRITE STARTED..."  

    i = 0
    s = state.db.size() 
    def toSave = state.valuestosave

    logging("values to save: $toSave")

    for(s!=0;i<s&&!state.paused;i++)
    {        
        def ref = state.db[i][2] 
        def dataFound = toSave.findAll{it == ref.toInteger()}
        boolean skip = ref.toInteger() in dataFound
        if(skip)
        {
            log.warn "value not replaced at user's request: ${ref} was found in ${toSave}" 
        }
        else 
        {
            //logging(".")
            state.db[i].remove(2)
            state.db[i] += baseline
        }
    }


    logging("BASE LINE UPDATE DONE!")

}
def createDb()
{
    log.info "CREATING DATABASE... this may take some ressources for a little while..."

    int h = 10 // minimum humidity value
    int sh = 100-h // humidity goes from 10 to 100%
    int i = -50 // minimum outside temp
    int s = 120 + Math.abs(i) // max outside temp expected, beyond that... move to Mars... 

    def db = []
    def insert = []
    long start = now()
    long timer = 120000
    for(s!=0;i<s;i++)
    {
        int a = 0
        h = 10
        for(sh!=0;a<s;a++) // all variations of humidity for every i (i being outside temp)
        { // s is max size = 120+abs(i) don't put <sh here
            if(h<100){h += 1}
            insert = [i,h,baseline] // create [[outside, humidity, setpoint]]
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
    log.debug "DONE! with  ${db.size()} entries and ${db.size()*3} sub-entries "

    //log.debug "db b = $db"
    state.db = db
    return db
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
