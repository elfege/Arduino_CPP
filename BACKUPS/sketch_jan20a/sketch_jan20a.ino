definition(
    name: "Thermostat Manager",
    namespace: "elfege",
    author: "ELFEGE",

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
                input "pw", "capability.powerMeter", title:"verify status with a power meter"

            }
            input "heatpump", "bool", title: "$thermostat is a heat pump or I want to add a secondary electric heater", submitOnChange:true
            if(heatpump)
            {
                input "heater", "capability.switch", title: "Select a switch to control an alternate heater", required: true, submitOnChange:true, multiple: false   
            }
            input "outsideTemp", "capability.temperatureMeasurement", title: "select a weather sensor for outside temperature", required:true, submitOnChange:true
            input "sensor", "capability.temperatureMeasurement", title: "select a temperature sensor (optional)", submitOnChange:true
            if(!sensor)
            {
                input "offrequiredbyuser", "bool", title: "turn off thermostat when desired temperature has been reached", defaultValue: false, submitOnChange:true
            }
            input "contact", "capability.contactSensor", title: "Turn off everything when these contacts are open", multiple: true, required: false, submitOnChange:true
            input "dimmer", "capability.switchLevel", title: "Use this dimmer as set point input source", required: true, submitOnChange:true


            input "outsideRef", "enum", title: "Select a temperature threshold reference method", required: true, options:["virtual dimmer", "static value"], submitOnChange:true
            if(outsideRef == "virtual dimmer")
            {                
                input "outsideThresDimmer", "capability.switchLevel", title: "Use this dimmer as a reference for outside temperature threshold", required: true, submitOnChange:true
            }
            else if(outsideRef == "static value")
            {
                input "outsideThreshold", "number", title: "type a numerical value representing the outside temperature threshold", required: true, submitOnChange:true, defaultValue:60 
            }
        }
        section("MOTION MANAGEMENT")
        {
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
            input "enabledebug", "bool", title: "Debug", submitOnChange:true
            if(enabledebug)
            {
                log.warn "debug enabled"             
                runIn(1800,logsOff)
                log.info "debug will be disabled in 30 minutes"
            }
            else 
            {
                log.warn "debug disabled"
            }
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
def initialize(){
    state.paused = false
    state.restricted = false
    state.lastNeed = "heat"
    state.boostVal = 85
    state.boostMode = boostMode
    state.boostAttempt = now() as long
        state.offAttempt = now() as long

        state.lastMotionEvent = now() as long
        state.motionEvents = 0

    logging("subscribing to events...")

    subscribe(location, "mode", ChangedModeHandler) 
    subscribe(thermostat, "temperature", temperatureHandler)
    if(sensor)
    {
        subscribe(sensor, "temperature", temperatureHandler)
    }
    if(outsideThresDimmer)
    {
        subscribe(outsideThresDimmer, "level", outsideThresDimmerHandler)
    }
    subscribe(dimmer, "level", dimmerHandler)
    log.info "subscribed $dimmer to dimmerHandler"
    subscribe(thermostat, "heatingSetpoint", setPointHandler)
    log.info "subscribed ${thermostat}'s heatingSetpoint to setPointHandler"
    subscribe(thermostat, "coolingSetpoint", setPointHandler)
    log.info "subscribed ${thermostat}'s coolingSetpoint to setPointHandler"

    if(motion)
    {
        subscribe(motion, "motion", motionHandler)
    }


    if(polldevices)
    {
        schedule("0 0/5 * * * ?", Poll)
    }

    schedule("0 0/1 * * * ?", mainloop)


    log.info "INITIALIZATION DONE"

}
/************************************************HANDLERS*********************************************************/
def appButtonHandler(btn) {
    switch(btn) {
        case "pause":state.paused = !state.paused
        logging("state.paused = $state.paused")
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
        if(!state.paused) mainloop()
        break
        case "poll":
        Poll()
        break

    }
}
def ChangedModeHandler(evt)
{
    logging("mode is $evt.value")

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
    logging("$evt.device is $evt.value")
    if(evt.value == "active")
    {
        logging("motion active...")
        state.motionEvents += 1    
        state.lastMotionEventDate = new Date().format("h:mm a", location.timeZone)  // format just for debug purpose

        state.lastMotionEvent = now() as long // use raw long value

            }
    mainloop()

}
def temperatureHandler(evt)
{
    logging("$evt.device returns ${evt.value}F")
    mainloop()
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
        logging("$dimmer set to $evt.value")
    }

    else if(!notBoostVal)
    {
        logging("BOOST VALUE, not adjusting dimmer")   
    }
    else 
    {
        logging("dimmer level ok (${dimmer.currentValue("level")} == ${evt.value}")
    }

    mainloop()
}
def dimmerHandler(evt)
{
    log.info "new dimmer level is $evt.value"
    mainloop()
}
def outsideThresDimmerHandler()
{
    log.info "Ouside threshold value is now: $evt.value"
    mainloop()

}
/************************************************MAIN loop*********************************************************/
def mainloop()
{

    def therMode = thermostat.currentValue("thermostatMode")
    logging("$thermostat is in $therMode mode 54dfg")

    if(therMode == "off")
    {
        state.boostOk = false
    }
    if(pw)
    {
        logging("$pw power meter returns ${pw.currentValue("power")}Watts")
    }
    if(therMode != "auto")
    {

        int desired = getDesired()
        def needData = getNeed(desired)
        def need = needData[1]
        logging("need is needData[1] = $need")
        state.boostVal = 85

        if(boost && need in ["heat", "cool"])
        {
            logging("BOOST MODE!")

            if(need == "heat")
            {
                logging("setting state.boostVal to heating boost value")
                state.boostVal = 85

            }
            else if(need == "cool")
            {
                logging("setting state.boostVal to cooling boost value")
                state.boostVal = 66
            }

            logging("state.boostVal = $state.boostVal")
            desired = state.boostVal

            if(boostMode.contains("()"))
            {
                // do nothing
            }
            else
            {
                boostMode + "()"
            }

            logging "boostMode command is: $boostMode"

            desired = state.boostVal
        }


        def cmd = "set"+"${needData[0]}"+"ingSetpoint"

        def currSP = thermostat.currentValue("thermostatSetpoint").toInteger()
        logging("therMode = $therMode currSP = $currSP")

        virtualThermostat(need)


        if(therMode != need)
        {
            if(need != "off" || (need == "off" && (sensor || offrequiredbyuser)))
            {
                //thermostat.setThermostatMode(need) // set desired mode
                logging("THERMOSTAT SET TO $need mode (587gf)")
                if(need == "off")
                {
                   // state.offAttempt = now() as long
                        }
            }
            else 
            {
                logging("THERMOSTAT stays in $therMode mode")
            }

        }
        else if(need != "off")
        {
            logging("Thermostat already set to $need mode")
        }

        logging("currSP != desired -> ${currSP != desired} -> ${currSP} != ${desired} ")
        if(need != "off" && currSP != desired)
        {
            if(boost)
            {
                pauseExecution(2000)
            }

            thermostat."${cmd}"(desired)   // set desired temp
            logging("THERMOSTAT SET TO $desired (564fdevrt)")
        }
        else if(need != "off")
        {
            logging("Thermostat already set to $desired")
        }

        if(boost && need != "off" && !state.boostOk)
        {
            state.boostOk = true
            state.boostAttempt = now() as long
                pauseExecution(5000)
            thermostat."${boostMode}"
            logging("THERMOSTAT SET TO $boostMode mode")
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
        if(pw)
        {
            // here we manage possible failure for a thermostat to have received the z-wave/zigbee or http command
            long timeElapsedSinceLastBoost = now() - state.boostAttempt
            long timeElapsedSinceLastOff = now() - state.offAttempt // when device driver returns state off while in fact signal didn't go through
            long threshold = 4 * 60 * 1000 // give it 4 minutes to kick in before attempting new request 
            boolean timeIsUp = timeElapsedSinceLastBoost > threshold
            boolean timeIsUpOff = timeElapsedSinceLastOff > 30000
            boolean pwLow = pw.currentValue("power") < 600
            logging("time since last boost Attempt = ${timeElapsedSinceLastBoost/1000} seconds & threshold = ${threshold/1000}sec")
            logging("time since last OFF Attempt = ${timeElapsedSinceLastOff/1000} seconds & threshold = ${30}sec")

            if(boost && timeIsUp && pwLow && need != "off")
            {
                log.warn "SELF HEALING ATTEMPT"
                state.boostAttempt = now() as long
                    logging("re-sending ${cmd}($desired)...")
                thermostat."${cmd}"(desired) // set the desired boost temperature value
                logging("waiting 10 seconds")
                pauseExecution(10000) // give plenty of times for respective commands to be received and processed by the device driver
                logging("sending $boostMode command") 
                thermostat."${boostMode}"// send the boost command as defined by the user
                pauseExecution(10000) // give plenty of times for respective commands to be received and processed by the device driver
                Poll()
            }
            else if(timeIsUpOff && need == "off" && !pwLow)
            {
                log.warn("$thermostat should be off but still draining power, resending cmd")
                state.offAttempt = now() as long
                    thermostat.setThermostatMode("off")
                thermostat.off()
                Poll()
            }
            else if(timeIsUp && need == "off" && !pwLow)
            {
                logging("$thermostat is off, skipping boost repair")   
            }
            else if((!pwLow &&  need in ["heat", "cool"]) || (need == "off" && pwLow))
            {
                log.info("EVERYTHING OK")
            }
            else 
            {
                log.warn("There's a problem... Auto Fix Should Kick in within time threshold")
            }
        }
    }
    else 
    {
        log.info("OVERRIDE MODE--------------")   
    }


}

def virtualThermostat(need)
{
    if(heatpump)
    {
        def outsideTemp = outsideTemp.currentValue("temperature") // only needed if electric heater here
        if(need == "heat" && outsideTemp < 40)
        {
            if(heater.currentValue("switch") != "on")
            {
                logging("Turning $heater on because outside temperature is currently ${outsideTemp}??F")
                heater.on()   
            }
            else 
            {
                logging("$heater is on because outside temperature is currently ${outsideTemp}??F")
            }
        }
        else 
        {
            if(heater.currentValue("switch") != "off")
            {
                logging("Turning $heater off")
                heater.off()
            }
            else 
            {

                logging("$heater already off")
            }
        }
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
    logging("contactClosed = $contactClosed")
    def outsideThres = getOutsideThershold()
    logging("outside Temperature Threshold is currently: $outsideThres")

    if(outsideTemp > outsideThres && inside >= desired + 0.5 && contactClosed && Active)
    {
        need0 = "Cool"// capital letter for later construction of the setCoolingSetpoint cmd
        need1 = "cool"
        state.lastNeed = need1
        logging("need and state.lastNeed set to ${[need0,need1]}")
    }
    else if(outsideTemp < outsideThres && inside <= desired - 0.5 && contactClosed && Active)
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
private getOutsideThershold()
{
    if(outsideThresDimmer)
    {
        return outsideThresDimmer.currentValue("level")
    }
    else if(outsideThreshold)
    {
        return outsideThreshold
    }
    else 
    {
        return 60
    }
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

        boolean heaterPoll = heater?.hasCommand("poll")
        boolean heaterRefresh = heater?.hasCommand("refresh") 

        if(heaterRefresh){
            heater.refresh()
            logging("refreshing $heater")
        }
        else if(heaterPoll){
            heater.poll()
            logging("polling $heater")
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

    if(motion)
    {
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
            logging("motion returns $result with ${state.motionEvents} active motion events in the last $noMotionTime minutes")
        }
        else 
        {
            logging("motion returns true because outside of motion modes")
        }
        logging("user did not select any motion sensor")
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


def logsOff(){
    log.warn "debug logging disabled..."
    app.updateSetting("enabledebug",[value:"false",type:"bool"])
}
