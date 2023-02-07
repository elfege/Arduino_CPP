/**
*  Copyright 2018 elfege
*
*  
*  Switch power 
*
*  Author: elfege
*/

definition(
    name: "Adjust Dimmer With Power Consumption",
    namespace: "elfege",
    author: "elfege",
    description: "Set a dimmer to a certain value based on a power meter and/or a temperature",
    category: "Convenience",
    iconUrl: "https://s3.amazonaws.com/smartapp-icons/Meta/light_outlet.png",
    iconX2Url: "https://s3.amazonaws.com/smartapp-icons/Meta/light_outlet@2x.png"
)


preferences {

    page name:"pageSetup"

}

def pageSetup() {

    if(state.paused)
    {
        logging("new app label: ${app.label}")
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

        logging("new app label: ${app.label}")
    }


    def pageProperties = [
        name:       "pageSetup",
        title:      "${app.label}",
        nextPage:   null,
        install:    true,
        uninstall:  true
    ]

    return dynamicPage(pageProperties) {
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
            paragraph " ", width: 6
            input "update", "button", title: "UPDATE", width: 6
            paragraph " ", width: 6
            input "run", "button", title: "RUN", width: 6
        }


        section("When power consumption passes a certain threshold, set a dimmer to a specific value") {
            input "pwmeter", "capability.powerMeter", multiple: true
            input "pwthreshold", "number", title: "power threshod"

            input "dimmer", "capability.switchLevel", title: "select a dimmer", multiple: false, description: "select a dimmer"
            input "dimLevel", "number", title: "When Power (or temperature) Threshold is passed, set to this level", description: "set a value for this dimmer"

            input "dimRestore", "number", title: "restore to this value when power is below threshold", description: "set a restore value"
        }

        section("Also adjust with temperature"){
            input "temperature", "capability.temperatureMeasurement", multiple: false, description: "select a temperature sensor", required: false
            input "tempThershold", "number", title: "Temperature threshold", description: "set temperature threshold", required: false

        }
        section("logging")
        {
            input "enablelogging", "bool", title:"Enable logging", value:false


        }
    }
}

def installed()
{
    logging("Installed with settings: ${settings}")
    initialize()
}

def updated()
{
    logging("Installed with settings: ${settings}")
   
    initialize()

}

def initialize() {
    unschedule()
    unsubscribe()

    if(enablelogging == true){
        runIn(1800, disablelogging)
        log.info "disablelogging scheduled to run in ${1800/60} minutes"
    }
    else 
    {
        log.warn "debug logging disabled!"
    }

    subscribe(pwmeter, "power", handler)

    if(temperature){
        subscribe(temperature, "temperature", handler)
    }
    schedule("0 0/5 * * * ?", Poll)

    mainloop()
}
def appButtonHandler(btn) {
    switch(btn) {
        case "pause":state.paused = !state.paused
        logging("state.paused = $state.paused")
        if(state.paused)
        {
            logging("unsuscribing from events...")
            unsubscribe()  
            logging("unschedule()...")
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
        if(!state.paused)  
        {
            mainloop()
        }
        else
        {
            log.warn "App is paused!"
        }
        break

    }
}

def handler(evt){
    log.info "evt.device returns $evt.value"
    mainloop()
}

def mainloop()
{

    boolean powerIsHigh = false
    boolean tempIsHigh = false


    if(pwmeter){

        int i = 0
        int s = pwmeter.size()
        def values = []
        for(s != 0; i < s; i++)
        {
            values << "${pwmeter[i].currentValue("power")}Watts"   
        }
        // check if One of multiple devices returns high power value    
        powerIsHigh = pwmeter.find{it.currentValue("power") >= pwthreshold}
        logging("$pwmeter return ${values} | powerIsHigh = $powerIsHigh")
    }
    if(temperature){
        def tempValue = temperature.currentValue("temperature")
        // check if One of multiple devices returns high power value    
        tempIsHigh = tempValue >= tempThershold
        logging("$temperature returns ${tempValue}°F | powerIsHigh = $tempIsHigh (tempValue = $tempValue)")
    }

    if(powerIsHigh || tempIsHigh)
    {
        dimmer.setLevel(dimLevel)
        logging("$dimmer set to $dimLevel ")
    }
    else {
        dimmer.setLevel(dimRestore)
        logging("$dimmer set to $dimRestore ")
    }

}

def Poll()
{


    if(polldevices)
    {
        boolean pwmeterPoll = pwmeter.hasCommand("poll")
        boolean pwmeterRefresh = pwmeter.hasCommand("refresh") 


        boolean temperaturePoll = temperature.hasCommand("poll")
        boolean temperatureRefresh = temperature.hasCommand("refresh")

        if(pwmeterRefresh){
            pwmeter.refresh()
            logging("refreshing $pwmeter")
        }
        else if(pwmeterPoll){
            pwmeter.poll()
            logging("polling $pwmeter")
        }
        if(temperatureRefresh){
            temperature.refresh()
            logging("refreshing $temperature")
        }
        else if(temperaturePoll){
            temperature.poll()
            logging("polling $temperature")
        }

    }

}

def logging(msg)
{
    def debug = settings.find{it.key == "enablelogging"}?.value
    //log.warn "debug = $debug"
    if (debug) log.debug msg
}

def disablelogging()
{
    app.updateSetting("enablelogging",[value:"false",type:"bool"])
    log.warn "logging disabled!"
}
