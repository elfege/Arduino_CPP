definition(
    name: "Thermostat Manager",
    namespace: "elfege",
    author: "WIFI_SSID",

    description: "Manage your thermostats with contact sensors, motion sensors and boost operations",

    category: "Green Living",
    iconUrl: "https://www.philonyc.com/assets/penrose.jpg",
    iconX2Url: "https://www.philonyc.com/assets/penrose.jpg",
    iconX3Url: "https://www.philonyc.com/assets/penrose.jpg", 
    image: "https://www.philonyc.com/assets/penrose.jpg"
)

preferences {

    page name: "settings"
}

/************************************************CONFIGURATION*********************************************************/
def settings() {

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
        while(app.label.contains(" (Paused) ")){app.updateLabel(app.label.minus("(Paused)" ))}
        log.debug "new app label: ${app.label}"
    }

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

    def pageProperties = [
        name:       "settings",
        title:      "Thermostats and other devices",
        nextPage:   null,
        install: true,
        uninstall: true
    ]

    dynamicPage(pageProperties) {
        
        section()
        {
            input "pause", "button", title: "$state.button_name"
            paragraph """


"""
        }

        section() {
            label title: "Assign a name", required: false
            input "restricted", "mode", title: "do not run this app while in these modes", multiple: true
        }

        section("Select the thermostat you want to control") { 

            input "thermostat", "capability.thermostat", title: "select a thermostat", required: true, multiple: false, description: null, submitOnChange:true
            input "boost", "bool", title: "boost this device", submitOnChange:true
            if(boost)
            {
                input "setCmd", "bool", title: "Add a custom command string", submitOnChange:true
                if(setCmd)
                {
                    input "boostMode", "text", title: "Write your command"
                }
            }
            input "outsideTemp", "capability.temperatureMeasurement", title: "select a weather sensor for outside temperature", required:true, submitOnChange:true
            input "sensor", "capability.temperatureMeasurement", title: "select a temperature sensor (optional)", submitOnChange:true
            input "contact", "capability.contactSensor", title: "Turn off ${Thermostat} when these contacts are open", multiple: true, required: false, submitOnChange:true
            input "dimmer", "capability.switchLevel", title: "Use this dimmer as set point input source", required: true, submitOnChange:true
            input "motion", "capability.motionSensor", title: "Save power when there's no motion", required: false, multiple: true, submitOnChange:true
            if(motion)
            {
                input "noMotionTime", "number", title: "after how long?", description: "enter a number in minutes"
                input "motionmodes", "mode", title: "Consider motion only in these modes", multiple: true, required: true 
            }            
        }

        section()
        {
            
            input "run", "button", title: "RUN"
            input "update", "button", title: "UPDATE"
            input "poll", "button", title: "REFRESH"
            input "polldevices", "bool", title: "Poll devices"
            input "enabledebug", "bool", title: "Debug"
        }

    }

}
def installed() {
    logging("Installed with settings: ${settings}")

    initialize()
}
def updated() {

    logging("""updated with settings: ${settings}-""")
    unsubscribe()
    unschedule()
    initialize()
}
def initialize()
{
    state.paused = false
    state.restricted = false
    state.lastNeed = "heat"
    state.boostVal = 85
    state.lastMotionEvent = now() as long
        state.motionEvents = 0

    logging("subscribing to events...")

    subscribe(location, "mode", ChangedModeHandler) 
    subscribe(thermostat, "temperature", temperatureHandler)
    if(sensor)
    {
        subscribe(sensor, "temperature", temperatureHandler)
    }
    subscribe(dimmer, "level", dimmerHandler)
    log.info "subscribed $dimmer to dimmerHandler"
    subscribe(thermostat, "heatingSetpoint", setPointHandler)
    log.info "subscribed ${thermostat}'s heatingSetpoint to setPointHandler"
    subscribe(thermostat, "coolingSetpoint", setPointHandler)
    log.info "subscribed ${thermostat}'s coolingSetpoint to setPointHandler"
    //subscribe(Thermostat, "thermostatSetpoint", setPointHandler)

    if(motion)
    {
        subscribe(motion, "motion", motionHandler)
    }


    if(polldevices)
    {
        schedule("0 0/5 * * * ?", Poll)
    }

    log.info "INITIALIZATION DONE"

}
/************************************************HANDLERS*********************************************************/
def appButtonHandler(btn) {
    switch(btn) {
        case "pause":state.paused = !state.paused
        log.debug "state.paused = $state.paused"
        if(state.paused)
        {
            log.debug "unsuscribing from events..."
            unsubscribe()  
            log.debug "unschedule()..."
            unschedule()
            break
        }
        else
        {
            initialize()            
            break
        }
        case "update":
        state.paused = false
        initialize()
        break
        case "run":
        if(!state.paused) evaluate()
        break
        case "poll":
        Poll()
        break

    }
}
def ChangedModeHandler(evt)
{
    log.info "mode is $evt.value"

    if(evt.value in restricted)
    {
        state.paused = true   
        state.restricted = true
    }
    else if(state.paused == true && state.restricted == true)
    {
        initialize()
    }
}
def motionHandler(evt)
{
    log.info "$evt.device is $evt.value"
    if(evt.value == "active")
    {
        logging("motion active...")
        state.motionEvents += 1    
        state.lastMotionEventDate = new Date().format("h:mm a", location.timeZone)  // format just for debug purpose

        state.lastMotionEvent = now() as long // use raw long value

            }
    evaluate()

}
def temperatureHandler(evt)
{
    logging("$evt.device returns ${evt.value}F")
    evaluate()
}
def setPointHandler(evt)
{
    log.info "new $evt.name is $evt.value---------------------------------------"

    def currDim = dimmer.currentValue("level")

    boolean notBoostVal = evt.value.toInteger() != state.boostVal.toInteger()

    logging (
        """state.lastNeed = $state.lastNeed   
evt.value = $evt.value   
state.boostVal = ${state.boostVal.toInteger()}  
notBoostVal: ${notBoostVal}""")



    if(currDim != evt.value && notBoostVal)
    {
        dimmer.setLevel(evt.value)
        log.info "$dimmer set to $evt.value"
    }

    else if(!notBoostVal)
    {
        logging("BOOST VALUE, not adjusting dimmer")   
    }
    else 
    {
        logging("dimmer level ok (${dimmer.currentValue("level")} == ${evt.value}")
    }

    evaluate()
}
def dimmerHandler(evt)
{
    log.info "new dimmer level is $evt.value"
    evaluate()
}
/************************************************MAIN FUNCTIONS*********************************************************/
def evaluate()
{

    def therMode = thermostat.currentValue("thermostatMode")
    logging("$thermostat is in $therMode")
    if(therMode != "auto")
    {
        int desired = getDesired()
        def needData = getNeed(desired)
        def need = needData[1]
        state.boostVal = 85

        if(boost && need in ["heat", "cool"])
        {
            logging("BOOST MODE!")

            if(need == "Heat")
            {
                state.boostVal = 85
            }
            else if(need == "Cool")
            {
                state.boostVal = 66
            }

            desired = state.boostVal

            if(boostMode.contains("("))
            {
                boostMode.minus("(")
            }
            if(boostMode.contains(")"))
            {
                boostMode.minus(")")
            }

            logging "boostMode command is: $boostMode"
            state.boostMode = boostMode
            desired = state.boostVal
        }


        def cmd = "set"+"${needData[0]}"+"ingSetpoint"

        def currSP = thermostat.currentValue("thermostatSetpoint")
        logging("therMode = $therMode currSP = $currSP")


        if(need != "off" && currSP != desired)
        {
            thermostat."${cmd}"(desired)   // set desired temp
            logging("THERMOSTAT SET TO $desired")
        }
        else if(need != "off")
        {
            logging("Thermostat already set to $desired")
        }
        if(therMode != need)
        {
            thermostat.setThermostatMode(need) // set desired mode
            logging("THERMOSTAT SET TO $need mode")
        }
        else if(need != "off")
        {
            logging("Thermostat already set to $need mode")
        }
        if(boost && need != "off" && !state.boostOk)
        {
            state.boostOk = true
            thermostat.setThermostatMode(state.boostMode)
            logging("THERMOSTAT SET TO $state.boostMode mode")
        }
        else 
        {
            if(need == "off")
            {
                state.boostOk = false
            }
            if(state.boostOk)
            {
                logging("Thermostat already set to $state.boostMode mode")
            }

        }
    }
    else 
    {
        logging("OVERRIDE MODE--------------")   
    }


}
/************************************************INPUTS*********************************************************/
def getDesired()
{
    int desired = 70
    if(!Active())
    {
        desired = dimmer.currentValue("level") - 6
    }
    else {
        desired = dimmer.currentValue("level")
    }

    logging("desired temperature is: $desired and current temperature is ${getInsideTemp()}")
    return desired
}
def getNeed(desired)
{
    int outsideTemp = outsideTemp.currentValue("temperature")
    def need0 = ""
    def need1 = ""
    def need = []
    def inside = getInsideTemp()
    boolean contactClosed = !contactsAreOpen()  

    if(outsideTemp > 60 && inside > desired && contactClosed && Active)
    {
        need0 = "Cool"// capital letter for later construction of the setCoolingSetpoint cmd
        need1 = "cool"
        state.lastNeed = need1
        loggging("need and state.lastNeed set to ${[need0,need1]}")
    }
    else if(outsideTemp < 60 && inside < desired && contactClosed && Active)
    {
        need0 = "Heat" // capital letter for later construction of the setHeatingSetpoint cmd
        need1 = "heat"
        state.lastNeed = need1
        logging("need and state.lastNeed set  to ${[need0,need1]}")
    }
    else 
    {
        need0 = "off"
        need1 = "off"
        logging("need set to OFF")
    }

    need = [need0, need1]
    logging ("need = $need")
    return need
}
def getInsideTemp()
{
    def inside = thermostat.currentValue("temperature")
    if(sensor)
    {
        inside = sensor.currentValue("temperature")
    }

    return inside
}
def Poll()
{


    if(polldevices)
    {
        boolean thermPoll = thermostat.hasCommand("poll")
        boolean thermRefresh = thermostat.hasCommand("refresh") 

        boolean outsidePoll = outsideTemp.hasCommand("poll")
        boolean outsideRefresh = outsideTemp.hasCommand("refresh")
        boolean override = state.override

        if(thermRefresh){
            thermostat.refresh()
            logging("refreshing $thermostat")
        }
        else if(thermPoll){
            thermostat.poll()
            logging("polling $thermostat")
        }
        if(outsideRefresh){
            outsideTemp.refresh()
            logging("refreshing $outsideTemp")
        }
        else if(outsidePoll){
            outsideTemp.poll()
            logging("polling $outsideTemp")
        }

        boolean sensorPoll = sensor?.hasCommand("poll")
        boolean sensorRefresh = sensor?.hasCommand("refresh") 

        if(sensorRefresh){
            sensor.refresh()
            logging("refreshing $sensor")
        }
        else if(sensorPoll){
            sensor.poll()
            logging("polling $sensor")
        }

    }

}
/************************************************BOOLEANS*********************************************************/
boolean contactsAreOpen() 
{
    def s = contact.size()
    def i = 0
    boolean Open = false
    for(s!=0;i<s;i++)
    {
        if(contact[i]?.currentValue("contact") == "open")
        {
            Open = true
            break
        }
    }
    logging("$contact open ?: $Open")
    return Open
}
boolean Active()
{
    boolean result = true // default is true  always return Active = true when no sensor is selected by the user

    if(location.mode in motionmodes)
    {
        if(motion)
        {
            long lastMotionEvent = state.lastMotionEvent as long
                long timeElapsedSinceLastMotionEvent = now() - lastMotionEvent
            long timeOutMillis = noMotionTime * 1000 * 60 
            int minutes = lastMotionEvent/1000/60
            logging("""lastMotionEvent < timeOutMillis  $lastMotionEvent < $timeOutMillis
log.info "last motion event occured at $state.lastMotionEventDate""")

            result = timeElapsedSinceLastMotionEvent < timeOutMillis

        }

        if(!result)
        {
            resetMotionEvents()
        }
        log.info "motion returns $result with ${state.motionEvents} active motion events in the last $noMotionTime minutes"
    }
    else 
    {
        log.info "motion returns true because outside of motion modes!"
    }
    return result
}
/************************************************OTHER*********************************************************/
def resetMotionEvents()
{
    log.info "No motion event has occured during the past $noMotionTime minutes"
    state.motionEvents = 0   
}
def logging(message)
{
    if(enabledebug)
    {
        log.debug message
    }
}
