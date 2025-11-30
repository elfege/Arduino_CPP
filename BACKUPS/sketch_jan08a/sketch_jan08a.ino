/**
*  
*
*  Author: WIFI_SSID
*/
definition(
    name: "DRY THE LAUNDRY",
    namespace: "elfege",
    author: "elfege",
    description: "Manage a heater and a dimmer/fan with humidity level",
    category: "Convenience",
    iconUrl: "http://static1.squarespace.com/static/5751f711d51cd45f35ec6b77/t/59c561cb268b9638e8ba6c23/1512332763339/?format=1500w",
    iconX2Url: "http://static1.squarespace.com/static/5751f711d51cd45f35ec6b77/t/59c561cb268b9638e8ba6c23/1512332763339/?format=1500w",
    iconX3Url: "http://static1.squarespace.com/static/5751f711d51cd45f35ec6b77/t/59c561cb268b9638e8ba6c23/1512332763339/?format=1500w",

)

preferences {
    page(name: "settings", title: "Set preferences", uninstal: true, install: true)
}

def settings(){
    
    if(state.paused)
    {
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

    
    dynamicPage(name: "settings", title: "Set preferences", uninstal: true, install: true){
        
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

        section("Set a dimmer to control the exhaust fan") {
            input "fan", "capability.switchLevel", title: "select a switch", required:true, multiple: false, submitOnChange: true      
        }
        section("Select a heater") {
            input "heater", "capability.switch", title: "select a switch", required:true, multiple: false, submitOnChange: true      

            input "addBlower", "bool", title: "add a fan/blower?", defaut: false, submitOnChange:true
            if(addBlower)
            {
                input "blower", "capability.switch", title: "select a switch", required:true, multiple: false, submitOnChange: true   
            }
            input "timerDelay", "number", title: "for how long?", description: "set time in hours"
        }
        section("Select Humidity Sensor") {
            input "humidity", "capability.relativeHumidityMeasurement", title: "pick a sensor", required:true, multiple: false, submitOnChange: true
        }
        section("Select temperature Sensor") {
            input "temperature", "capability.temperatureMeasurement", title: "pick a sensor", required:true, multiple: false, submitOnChange: true
        }
        section("Trigger this app when this switch is turned on") {
            input "trigger", "capability.switch", title: "select a switch", required:false, multiple: true, submitOnChange: true   
        }
        section("Pause $fan speed modulations when this switch is turned on") {
            input "pauseSw", "capability.switch", title: "select a switch", required:false, multiple: true, submitOnChange: true   
        }

        section([mobileOnly:true]) {
            label title: "Assign a name", required: false
            mode title: "Set for specific mode(s)", required: false, uninstall: true
        }

        section("")
        {
            //input "pause", "button", title: "$state.button_name"

            input "pause", "button", title: "$state.button_name"
            paragraph " ", width: 6
            input "update", "button", title: "UPDATE", width: 6
            paragraph " ", width: 6
            input "run", "button", title: "RUN", width: 6
        }
        
        section()
        {
            input "enablelogging", "bool", title: "log debug", submitOnChange: true, defaultValue: false
            if(enablelogging)
            {
                log.warn "debugging is on"
                
            }
        }
    }
}

def installed() {
    initialize()

}

def updated() {
    unsubscribe()
    unschedule()
    initialize()

}

def initialize() {

    runIn(600, debugOff)
    
    setThresholds()
    state.lastOn = now() as Long
    state.turnedBackOn = false

    subscribe(trigger, "switch", triggerHandler)
    subscribe(heater, "switch", heaterSwitchHandler)
    if(blower)
    {
        subscribe(blower, "switch", heaterSwitchHandler)
    }
    if(pauseSw)
    {
        subscribe(pauseSw, "switch", pauseSwitchHandler)
    }
    subscribe(fan, "level", dimmersHandler)

    subscribe(humidity, "humidity", humidityHandler)
    subscribe(temperature, "temperature", temperatureHandler)


    schedule("0 0/1 * * * ?", telltemp)


    logging("""initialized with settings: $settings""")
}

def triggerHandler(evt){
    logging("$evt.device turned $evt.value")

    boolean alldone = state.alldone

    if(evt.value == "on")
    {
        state.alldone = false // allows to detect premature shut off
        heater.on()
        if(blower){
            blower.on()
            logging("blower turned on")
        }
        state.lastOn = now()
    }
    else if(evt.value == "off")
    {
        // off has been requested, exhaust all remaining humid air untill user or another app takes over 
        heater.off()
        if(blower){ 
            def blowTimer = 60 // in minutes
            runIn(blowTimer * 60, blowerOff)
            logging("blower turned off in ${blowTimer} minutes")
        }
        state.turnedBackOn = false
    }

}

def dimmersHandler(evt) {
    logging("$evt.device set to $evt.value")

}

def heaterSwitchHandler(evt){
    logging("$evt.device turned $evt.value")
}

def pauseSwitchHandler(evt){
    logging("PAUSE SWITCH IS $evt.value")

    if(evt.value == "on")
    {
        state.fanPaused = true
    }
    else 
    {
        state.fanPaused = false
    }
}

def humidityHandler(evt){
    logging("$evt.device returns $evt.value")
}

def temperatureHandler(evt){
    logging("$evt.device returns $evt.value")

}

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
        if(!state.paused) mainloop()
        break

    }
}

def telltemp()
{

    int s = trigger.size()
    int i = 0 
    boolean triggered = false
    for(s!=0;i<s && !triggered;i++)
    {
        triggered = trigger[i].currentValue("switch") == "on"      
    }
    logging("triggered returns $triggered")

    boolean heaterIsOn = heater.currentValue("switch") == "on"
    if(blower){
        boolean blowerIsOn = blower?.currentValue("switch") == "on"
    }
    def temp = temperature.currentValue("temperature").toFloat().toInteger()
    def humval = humidity.currentValue("humidity").toInteger()
    def fanval = fan.currentValue("level")

    if(heaterIsOn || blowerIsOn){
        fan.setLevel(getFanSpeed(temp, humval))
    }

    long timer = timerDelay * 60 // in minutes
    long timeDelay = timer * 60 * 1000  // * 60 = in seconds * 1000 = in millisecs
    if(blower)
    {
        // if we have a blower, devide time by 2
        timeDelay = timeDelay / 2
    }
    long now = now()
    if(now - state.lastOn > timeDelay)
    {
        if(triggered){
            log.info "END OF DRYING"
            trigger.off()
        }
        else {
            logging("$trigger already turned off")
        }
    }

    if(triggered && !heaterIsOn && !state.turnedBackOn)
    {
        logging("It looks like $heater has been turned off unexpectedly, turning it back on")
        state.turnedBackOn = true
        runIn(2, heaterBackOn) // delay to prevent cross commands
    }
    else if(triggered && !heaterIsOn && state.turnedBackOn)
    {
        logging("USER OVERRIDE!")
        trigger.off()
    }

    def heaterPower = "Not Capable" 
    def blowerPower = "Not Capable" 
    def heaterPowerCap = heater.hasCapability("Power") || heater.hasCapability("power")  || heater.hasCapability("Power Meter") 
    def blowerPowerCap = blower?.hasCapability("Power") || blower?.hasCapability("power")  || blower?.hasCapability("Power Meter")

    if(heaterPowerCap)
    {
        heaterPower = heater.currentValue("power")
    }
    else {
        logging("$heater has no power measurement capability")
    }
    if(blowerPowerCap)
    {
        blowerPower = blower.currentValue("power")
    }
    else {
        logging("$blower has no power measurement capability")
    }

    long elapsed = (now - state.lastOn)
    long HoursElapsed   = elapsed / 1000 / 60 / 60
    long MinutesElapsed = elapsed / 1000 / 60 - (HoursElapsed * 60)
    long SecondsElapsed = elapsed / 1000 - (MinutesElapsed * 60)

    long HoursTimeLeft  = 0 
    if(((timeDelay - elapsed)/1000/60/60) > 0){HoursTimeLeft = (timeDelay - elapsed)/1000/60/60}
    long MminutesTimeLeft   = 0 
    if(((timeDelay - elapsed)/1000/60) > 0)
    {
        MminutesTimeLeft = ((timeDelay - elapsed) - (HoursTimeLeft*1000*60*60)) / 1000 / 60
    }
    long SecondsTimeLeft = 0
    if(((timeDelay - elapsed)/1000) > 0)
    {
        SecondsTimeLeft = ((timeDelay - elapsed) - (MminutesTimeLeft*1000*60) - (HoursTimeLeft*1000*60*60)) / 1000 
    }

    def TimeLeft = "${HoursTimeLeft}:${MminutesTimeLeft}:${SecondsTimeLeft}"


    logging("""
Temperature: ${temp}
Humidity: ${humval}
heaterPower = ${heaterPower}
blowerPower: ${blowerPower}
Fan at ${fanval}%
triggered = $triggered

elapsed time: ${HoursElapsed}:${MinutesElapsed}:${SecondsElapsed}
SecondsElapsed: 
Time Left: : ${TimeLeft}

""")
}
// runIn(3, telltemp) // FOR TESTS ONLY D




def setThresholds()
{
    state.baseLevel = 30
    state.highLevel = 100
    state.humidityThreshold = 50
    state.temperatureThreshold = 80
}

def getFanSpeed(int temp, int hum)
{

    if(!state.fanPaused)
    {
        def xa = 150 // high temperature level, when we need less heat and more fan 
        def xb = 20 // low temperature level, when we need more heat and less fan  
        def ya = state.highLevel // high speed fan 
        def yb = state.baseLevel // low speed fan

        //get the slope 
        def slope = (yb-ya)/(xb-xa)
        //temp = 90
        //hum = 30
        // get the constant
        int b = ya - slope * xa // solution to ya = coef*xa + b // newSpeed = slope*temp + b

        //int b = 2*(temp/hum)// twice the temp to humidity ratio as a ponderator 

        int newSpeed = slope*temp + b

        logging("slope = $slope, b = $b, temp = $temp, hum = $hum, newSpeed = $newSpeed")

        return newSpeed

    }
    else 
    {
        log.info "fan speed modulation paused because $pauseSw is on"
    }

}

def heaterBackOn()
{
    heater.on()
    blower?.on()
}

def blowerOff()
{
    blower?.off();
    logging("turning off $blower")
}

def debugOff(){
    log.warn "debug logs disabled..."
    device.updateSetting("enablelogging",[value:"false",type:"bool"])
}

private logging(msg) {
    if (settings?.enablelogging || settings?.enablelogging == null) {
        log.debug "$msg"
    }
}
