import java.text.SimpleDateFormat
import groovy.transform.Field
import groovy.json.JsonOutput

@Field static remoteResponded = true



definition(
    name: "CSP Watchdog",
    namespace: "elfege",
    author: "ELFEGE",

    description: """Check that cron service is running properly and or that a switch is responding in a timely fashion
and/or a remote hub is responsive, if not, then reboot the hub (or the remote hub)""",

    category: "maintenance",
    iconUrl: "https://www.philonyc.com/assets/penrose.jpg",
    iconX2Url: "https://www.philonyc.com/assets/penrose.jpg", 
    image: "https://www.philonyc.com/assets/penrose.jpg"
)

preferences {
    page name: "pageConfig"
    page name: "connectPage"
}
mappings{
    // Server Mappings
    path("/ping")
    {
        action: [GET: "parsePing"]
    }
    path("/confirmReception")
    {
        action: [GET: "confirmationReceived"]
    }
    path("/rebooting")
    {
        action: [GET: "registerReboot"]
    }

}
def pageConfig() {

    appLabel()

    def pageProperties = [
        name:       "pageConfig",
        title:      "$app.label",
        nextPage:   null,
        install: true,
        uninstall: true,
        submitOnChange:true
    ]

    dynamicPage(pageProperties) {


        if(atomicState.paused == true)
        {
            atomicState.button_name = "resume"
            logging("button name is: $atomicState.button_name")
        }
        else 
        {
            atomicState.button_name = "pause"
            logging("button name is: $atomicState.button_name")
        }


        section("")
        {
            input "pause", "button", title: "$atomicState.button_name"
        }
        section("restricted modes")
        {
            input "restricted", "mode", title: "Do not run this app when in these modes:", description:"select modes", multiple:true, required:false
        }

        section(menuHeader("Method 1: Motion Sensors Trigger - this is the basic method"))
        { 
            input "motionSensors", "capability.motionSensor", title: "Choose your motion sensors", despcription: "pick a motion sensor", required:true,
                multiple:true, submitOnChange: true
            input "ip", "text", title: "IP ADDRESS OF YOUR HUB", despcription: "enter your ip address without 'http://'", required:true
        }
        section(menuHeader("Method 2: Virtual Switch trigger - this method provides better reactivity"))
        {
            input "swt", "capability.switch", title: "(optional) Select a control virtual switch",description:"Select a VIRTUAL switch", required:false, submitOnChange:true
            if(swt)
            {
                paragraph "This app will monitor the reaction time between the cmd and the event declaration to check your hub's reactivity."
                input "repeatswitch", "number", title: "Set reboot sensitivity (number of failed tests before reboot)", required: true, range: "3.. 100", submitOnChange:true
            }
        }
        section(menuHeader("Method 3: Local Remote Hub Trigger - this method is great for those whom use a second hub"))
        {
            input "workWithRemoteHub", "bool", title: "watch a remote hub on your network", defaultValue: false, submitOnChange: true
            if(workWithRemoteHub)
            {
                input "repeatremote", "number", title: "After how many failed attempts should this app reboot the remote server?", description: "Set a value between 3 and 30", range: "3..30",defaultValue: "5"
                href "connectPage", title: "Configure IP and Token Key", description: "", state: atomicState.clientURI ? "complete" : null
            }
        }

        section("logging")
        {
            input "enablelogging", "bool", title:"Enable logging", defaultValue:false, submitOnChange:true
            input "enabledescriptiontext", "bool", title:"Enable description text", defaultValue:true, submitOnChange:true
        }
        section()
        {
            input "userLabel", "text", title: "Rename this app", submitOnChange: true, required: false

            if(atomicState.installed)
            {  

                if(atomicState.noReboot == null || atomicState.noReboot == "null"){atomicState.noReboot = false}
                input "update", "button", title: "UPDATE"
                input "noreboot", "button", title: "${atomicState.noReboot ? "Enable Reboot" : "Disable Reboot"}"
                if(atomicState.noReboot){
                    paragraph "<div style=\"width:102%;background-color:#1C2BB7;color:red;padding:4px;font-weight: bold;box-shadow: 1px 2px 2px #bababa;margin-left: -10px\">${atomicState.messageReboot}</div>"
                }
                else 
                { 
                    paragraph ""
                }
                input "reboot", "button", title: "REBOOT $location"
                if(swt) {
                    input "test", "button", title: "toggle $swt"
                }
                input "run", "button", title: "RUN"
                if(workWithRemoteHub) {
                    input "pingtest", "button", title: "PING TEST"
                    input "RemoteHubreboot", "button", title: "Reboot ${clientName} (remote hub)"
                    if(atomicState.noReboot){
                        paragraph "<div style=\"width:102%;background-color:#1C2BB7;color:red;padding:4px;font-weight: bold;box-shadow: 1px 2px 2px #bababa;margin-left: -10px\">${atomicState.messageReboot}</div>"
                    }
                    else 
                    {
                        paragraph ""
                    }
                }
            }
        }
    }
}
def connectPage(){
    if (!state?.accessToken)
    {
        createAccessToken()
    }

    atomicState.remoteType = "local"
    atomicState.localConnectionType = http

    dynamicPage(name: "connectPage", uninstall: false, install: false)
    {
        section()
        {
            paragraph "<h2> Make sure to enable the same settings on your other hub, using the same app </h2>"
        }

        section("Client Details")
        {
            input "clientName", "string", title: "Friendly Name of Remote Client:", required: false, defaultValue: null, submitOnChange: true
            if (clientName) input "clientIP", "string", title: "Private LAN IP Address of Remote Client:", required: false, defaultValue: null, submitOnChange: true
        }
        if (clientIP)
        {
            section("Connection Key")
            {


                paragraph("Copy and Paste this Connection Key into the Remote hub's configuration:<textarea rows=\"3\" style=\"width:80%; font-family:Consolas,Monaco,Lucida Console,Liberation Mono,DejaVu Sans Mono,Bitstream Vera Sans Mono,Courier New, monospace;\" onclick=\"this.select()\" onclick=\"this.focus()\">${getConnectString()}</textarea>")


            }

            section("connection key")
            {   
                input "serverKey", "string", title: "Paste here the connection key you've gotten from the other hub's instance of this app:", required: true
            }
        }
    }
}
def appButtonHandler(btn) {
    switch(btn) {
        case "pause":atomicState.paused = !atomicState.paused
        log.debug "atomicState.paused = $atomicState.paused"
        if(atomicState.paused)
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
        atomicState.paused = false
        updated()
        break
        case "test":
        atomicState.swtCmd = now()
        testSwitch() // leave time to atomicState.swtCmd value to be written 
        break
        case "reboot":
        if(atomicState.noReboot)
        {
            log.warn "REBOOT WAS DISABLED BY USER"
            atomicState.messageReboot = "REBOOT DISABLED BY USER, COMMAND NOT SENT!"
        }
        else 
        {
            atomicState.messageReboot = ""
            reboot()
        }
        case "RemoteHubReboot":
        if(atomicState.noReboot)
        {
            log.warn "REBOOT WAS DISABLED BY USER"
            atomicState.messageReboot = "REBOOT DISABLED BY USER, COMMAND NOT SENT!"
        }
        else 
        {
            atomicState.messageReboot = ""
            rebootRemoteHub()
        }
        atomicState.confirm = false
        break
        case "CANCEL":
        atomicState.confirm = false
        break
        case "pingtest":
        remoteServerHealth()
        break
        case "run":
        master("testrun")
        break
        case "noreboot":

        atomicState.noReboot = !atomicState.noReboot
        if(atomicState.noReboot)
        {
            log.warn "REBOOT DISABLED BY USER"
            atomicState.messageReboot = "REBOOT DISABLED BY USER"
        }
        else 
        {
            atomicState.messageReboot = ""
        }

        break

    }
}
def installed() {
    logging("Installed with settings: ${settings}")
    atomicState.fix = 0
    atomicState.lastReboot = now()
    atomicState.installed = true
    init()
}
def updated() {
    log.info "updated with settings: ${settings}"

    unsubscribe()
    unschedule()
    init()
}

def init() {
    atomicState.messageReboot = ""
    atomicState.paused = false
    atomicState.pausedRemoteReboot = false
    atomicState.remoteRebootTime = now()
    atomicState.lastLabelTime = now()
    atomicState.swtEvt = 0
    //atomicState.remoteResponded = true // using static instead
    atomicState.pingInterval = 60000 // default
    atomicState.lastCheckIn = now()
    atomicState.attempts = 0
    atomicState.cronPositive = 0
    atomicState.swtAttempt = 0
    atomicState.lastRun = now() // time stamp to see if cron service is working properly
    atomicState.lastRunStamp = new Date().format("h:mm:ss a", location.timeZone) // formated time stamp for debug purpose


    if(enablelogging == true){
        atomicState.EnableDebugTime = now()
        runIn(1800, disablelogging)
        log.info "disablelogging scheduled to run in ${1800/60} minutes"
    }
    else 
    {
        log.warn "debug logging disabled!"
    }

    int i = 0
    int s = 0

    i = 0
    s = motionSensors.size()
    for(s!=0;i<s;i++)
    {
        subscribe(motionSensors[i], "motion", mainHandler)
        log.trace "${motionSensors[i]} subscribed to events"
    }

    if(swt)
    {
        subscribe(swt, "switch", switchHandler)
        atomicState.lastSwtEvent = now() 
        atomicState.swtCmd = now()
    }

    configureRemote()

    subscribe(location, "systemStart", hubEventHandler) // manage bugs and hub crashes


    atomicState.timer = 1
    schedule("0 0/${atomicState.timer} * * * ?", cronMaster) 

    logging("initialization done")
}
def locationModeChangeHandler(evt){
    logging("$evt.name is now in $evt.value mode")   
}
def switchHandler(evt){
    log.info "$evt.device is $evt.value"


    atomicState.swtEvt = 0 // reset this value to 0 so the app knows that hub is responsive to events (boolean is an insufficient method here after a fresh reboot)

    def timeTolerance = 2 // in seconds
    def elapsed = (now() - atomicState.swtCmd) / 1000 // elapsed time converted into seconds
    def falseAlarmThreshold = repeatswitch ? repeatswitch : 3 // max number of times the elapsed time can have been measured beyond timeTolerance
    def previousFAThr = falseAlarmThreshold
    def absoluteLimit = 10 // (in seconds) too much of a delay is not acceptable so if this limit is passed...
    //... then bring false alarm threshold down 
    if(elapsed >= absoluteLimit) {
        // if it happens a second time that it took too long for the hub to parse a switch event, then reboot the hub
        falseAlarmThreshold = 2 // this value will be kept if and only if absolute limit has been passed more than twice
        atomicState.swtAttempt += 1
        log.warn "delay is $elapsed seconds --> HUB IS GETTING TOO SLOW bringing falseAlarmThreshold from $previousFAThr to $falseAlarmThreshold; current attemps: $atomicState.swtAttempt" 
    } 
    else
    {
        def message = ""
        if(falseAlarmThreshold != previousFAThr)
        {
            falseAlarmThreshold = repeatswitch ? repeatswitch : 3
            log.warn "FALSE POSITIVE SWITCH TEST, elapsed time is now back to within acceptable value; restoring orignal false alarm threshold of $falseAlarmThreshold attemps"
        }
        else
        {
            descriptiontext "Switch event elapsed time is below critical"
        }
    }
    atomicState.lastSwtEvent = now() // remember when last event was registered

    logging """
timeTolerance = ${timeTolerance/1000} seconds
elapsed time between cmd and evt = $elapsed seconds
absoluteLimit = $absoluteLimit
falseAlarmThreshold = $falseAlarmThreshold (from settings: $repeatswitch)
"""

    if(elapsed > timeTolerance && elapsed < absoluteLimit)
    {
        log.warn "HUB TOOK ${(now() - atomicState.swtCmd)/1000} seconds TO RESPOND TO CMD attempt #${atomicState.swtAttempt}"
        atomicState.swtAttempt += 1
        // run a new test
        atomicState.swtCmd = now()
        testSwitch()
    }
    else  if(elapsed < timeTolerance)
    {
        logging "$elapsed < $timeTolerance -- ${elapsed < timeTolerance}"
        descriptiontext("Local Hub Response time to a Switch Command is within acceptable parameter (${elapsed} seconds)")
        atomicState.swtAttempt = 0
    }

    if(atomicState.swtAttempt >= falseAlarmThreshold) 
    {
        log.warn """
REBOOTING this hub because it took too long to respond to switch cmds on too many occasions...
atomicState.swtAttempt = $atomicState.swtAttempt
elapsed = $elapsed (absoluteLimit is $absoluteLimit)
"""
        atomicState.swtAttempt = 0
        reboot()
        return
    }
}
def mainHandler(evt){    
    //log.info "${evt.name}: $evt.device is $evt.value --------------------"

    if(location.mode in restrictedModes)
    {
        logging("location in restricted mode, doing nothing")
        return
    }
    else 
    {
        master("mainHandler")
    }
}
def hubEventHandler(evt){

    unschedule()
    unsubscribe() // temporarily stop all instances to prevent loop reboots

    log.warn "Hub has just restarted! ${app.label} will resume in 2 minutes..."
    atomicState.lastReboot = now()
    runIn(120, updated) // restart this app 2 minutes after complete reboot
    atomicState.pausedRemoteReboot = false
}
def cronMaster(){
    atomicState.lastRun = now() // time stamp to see if cron service is working properly
    atomicState.lastRunStamp = new Date().format("h:mm:ss a", location.timeZone) // formated time stamp for debug purpose
    master("cronMaster")
}
def master(String data){

    //disableZwave()

    if(atomicState.paused)
    {
        if(atomicState.pausedRemoteReboot)
        {
            log.info "app paused due to remote reboot procedure. It will resume in a couple minutes, be patient"
        }
        else
        {
            log.info "app paused"
            runIn(1800, updated)
        }
    }
    else 
    {

        if(data == "cronMaster" && atomicState.cronPositive != 0)
        {
            log.warn("FALSE POSITIVE CRON TEST, reboot canceled!")
            atomicState.cronPositive = 0
        }

        if(workWithRemoteHub)
        {
            logging "atomicState.pingInterval = $atomicState.pingInterval"
            if(now() - atomicState.lastCheckIn > atomicState.pingInterval)
            {
                atomicState.lastCheckIn = now()
                remoteServerHealth()
                //descriptiontext("checking if remote hub responded")
                runIn(2, checkResult)

                if(now() - atomicState.lastLabelTime > 120000 || remoteResponded == false) 
                {
                    appLabel()
                }
            }
        }

        if(swt) {
            def elapsed = now() - atomicState.swtCmd
            logging("elapsed time between swt cmd and execution = ${elapsed}")


            if(elapsed > 30000)
            {

                if(atomicState.swtEvt > 2 && now() - atomicState.lastSwtEvent > 60000)
                {
                    log.warn "Hub is no longer responding to events! REBOOTING"
                    reboot()
                }
                
                atomicState.swtCmd = now()
                testSwitch() // leave time to atomicState.swtCmd value to be written 
                atomicState.swtEvt += 1
            }
        }




        logging("***${new Date().format("h:mm:ss a", location.timeZone)}**** Origin: $data")

        if(location.mode in restrictedModes)
        {
            log.info("location in restricted mode, doing nothing")
        }
        else 
        {
            long t = atomicState.timer
            long d = t * 60000 + 30000 // allow up to n millis delay for cron to execute
            if(now() - atomicState.lastRun > d) // if this loop hasn't run for more than the timer's value, then it's probably that CRON is broken
            {
                log.warn "CRON SERVICE IS NOT WORKING! #$atomicState.cronPositive Current time: ${new Date().format("h:mm:ss a", location.timeZone)} last sched run: ${atomicState.lastRunStamp}" 
                atomicState.cronPositive += 1

                if(now() - atomicState.lastReboot > 15 * 60 * 1000)
                {
                    if(atomicState.cronPositive > 3)
                    {
                        log.warn "*******************REBOOT **************"

                        reboot()
                    }
                }
                else 
                {
                    log.warn "LAST REBOOT WAS NOT SO LONG AGO, SKIPPING"
                    atomicState.cronPositive = 0
                }

            }
            else 
            {
                if(atomicState.cronPositive != 0)
                {
                    log.warn "FALSE CRON FAILLURE ALARM"
                    atomicState.cronPositive = 0
                }
            }
        }
    }
}

def testSwitch()
{
    descriptiontext("Testing Hub's Responsiveness by toggling $swt")
    (swt.currentValue("switch") == "off") ? swt.on() : swt.off()
}

def disableZwave(){
    log.warn "DISABLING Z-WAVE NETWORK"

    def cmd = ""
    def port = 8080
    def path = "/hub/zwave/update"
    def uri = "http://${ip}${":"}${port}${path}/?id=1&version=1&locationId=1&zwaveSecure=0&zwaveStatus=enable&_action_update=Update"
    log.debug "GET: $uri"

    def requestParams =
        [
            uri:  uri,
            requestContentType: "application/json",
            timeout: 5
        ]

    try
    {
        asynchttpGet((enableDebug ? "asyncHTTPHandler" : null), requestParams)
    }
    catch (Exception e)
    {
        log.error "asynchttpGet() failed with error ${e.message}"
    }

}
def reboot(){
    log.warn "NOW REBOOTING THE HUB"
    if(workWithRemoteHub){sendGetCommand("/rebooting")}
    if(!atomicState.noReboot)
    {
        runCmd("${ip}", "8080", "/hub/reboot")// reboot
    }   
    else
    {
        log.warn "REBOOT CMD DISABLED!"
    }

}
def rebootRemoteHub(){
    log.warn "----------------- REBOOTING ${clientName} ---------------------- "
    if(!atomicState.noReboot)
    {
        runCmd("${clientIP}", "8080", "/hub/reboot")// reboot
    }
    else
    {
        log.warn "REBOOT CMD DISABLED!"
    }

    // pause this app 
    unschedule()
    unsubscribe() // temporarily stop all instances to allow for the remote hub to start responding again
    atomicState.pausedRemoteReboot = true // next ping received from other hub will switch back this boolean
    atomicState.paused = true
    runIn(300, updated) // resume this app in a couple minutes to leave time for the other hub to reboot 
}
def runCmd(String ip,String port,String path) {

    def uri = "http://${ip}${":"}${port}${path}"
    log.debug "POST: $uri"

    def reqParams = [
        uri: uri
    ]

    try {
        httpPost(reqParams){response ->
        }
    } catch (Exception e) {
        log.error "${e}"
    }
}
def remoteServerHealth(){
    descriptiontext("Remote Server Health Check")
    remoteResponded = false

    logging "atomicState.pausedRemoteReboot = $atomicState.pausedRemoteReboot"
    def message = atomicState.pausedRemoteReboot ? ("ping activity suspended because  ${clientName} is rebooting") : ("") //{sendGetCommand("/ping")}
    if(message){log.warn (message)}
    if(!atomicState.pausedRemoteReboot){sendGetCommand("/ping")}

}
def parsePing(){ // receive ping from remote server
    if(atomicState.paused && !atomicState.pausedRemoteReboot)
    {
        log.info "ping activity paused"
    }
    else 
    {
        descriptiontext("Received ping from ${clientName}.")
        if(atomicState.pausedRemoteReboot){
            // receiving ping from other hub so it is done rebooting, resuming this app
            updated()
        }
    }
    // send confirmation to the remote hub 
    sendConfirmation()
    jsonResponse([status: "received"])
}
def checkResult(){

    logging """
remoteResponded = $remoteResponded 
atomicState.attempts = $atomicState.attempts
repeatremote = $repeatremote
"""
    if(remoteResponded == false && atomicState.attempts < repeatremote)// prevent false alarm 
    {
        if(atomicState.attempts != 0) {log.warn "${clientName} failed to respond! attempt #$atomicState.attempts"} //by delcaring faillure only as of 2 failed attempts
        atomicState.attempts += 1
        atomicState.pingInterval = 3000 // set shorter ping interval 
        runIn(30, remoteServerHealth) 
    }
    else if(remoteResponded == false && atomicState.attempts >= repeatremote)
    {
        rebootRemoteHub()
    }
    else if(remoteResponded == true)
    {
        if(atomicState.attempts != 0){log.warn "FALSE ALARM, resuming normal operations"}
        atomicState.attempts = 0
        if(atomicState.pingInterval != 60000)
        {
            atomicState.pingInterval = 60000 // reset ping interval
        }
    }
    else 
    {
        log.error "Something went wrong, could not resolve reboot conditions..."
    }
}
def registerReboot(){
    log.warn "OTHER HUB is rebooting on its own, stopping all ping activities"
    atomicState.pausedRemoteReboot = true
    atomicState.paused = true
}
def sendConfirmation(){
    sendGetCommand("/confirmReception")
}
def sendGetCommand(command){

    def serverURI = atomicState.clientURI + command

    logging("sending $serverURI")

    def requestParams =
        [
            uri:  serverURI,
            requestContentType: "application/json",
            headers:
            [
                Authorization: "Bearer ${atomicState.clientToken}"
            ],
            timeout: 5
        ]

    try
    {
        asynchttpGet((enableDebug ? "asyncHTTPHandler" : null), requestParams)
    }
    catch (Exception e)
    {
        log.error "asynchttpGet() failed with error ${e.message}"
    }
}
def confirmationReceived(){

    remoteResponded = true
    if(atomicState.attempts != 0){
        log.warn "FALSE ALARM ${clientName} is online"
    } else {
        descriptiontext"CONNECTION TO ${clientName} IS HEALTHY"
        logging "atomicState.attempts RESET"
    }
    // declare false alarm only if 2 attempts were already made 

    atomicState.attempts = 0
    runIn(1, appLabel)
}

def asyncHTTPHandler(response, data){
    if (response?.status != 200)
    {
        log.error "asynchttpGet() request failed with error ${response?.status}"
    }
}

def jsonResponse(respMap){
    render contentType: 'application/json', data: JsonOutput.toJson(respMap)
}
def configureRemote(){
    def connectString = serverKey
    if (connectString == null) {return}

    def accessData
    try
    {
        accessData = parseJson(new String(connectString.decodeBase64()))
    }
    catch (errorException)
    {
        log.error "Error reading connection key: ${errorException}."
        responseText = "Error: Corrupt or invalid connection key"
        atomicState.connected = false
        accessData = null
    }
    if (accessData)
    {
        // Set the coordinator hub details
        atomicState.clientURI = accessData.uri
        atomicState.clientToken = accessData.token
        atomicState.clientType = accessData.type
        atomicState.connectionType = accessData.connectionType

        log.info """
atomicState.clientURI = $atomicState.clientURI
atomicState.clientToken = $atomicState.clientToken
atomicState.clientType = $atomicState.clientType
atomicState.connectionType = $atomicState.connectionType
"""
    }
}
def appLabel(){
    atomicState.lastLabelTime = now()
    def appLa = userLabel ? userLabel : "CSP WATCHDOG"  

    appLa = atomicState.paused ? appLa + ("<font color = 'red'> paused </font>") : appLa

    if(workWithRemoteHub)
    {
        appLa = remoteResponded ? appLa + ("<font color = 'red'> online </font>"):appLa + ("<font color = 'red'> offline </font>")  
        appLa = atomicState.pausedRemoteReboot ? appLa + ("<font color = 'red'> $clientName rebooting </font>") : appLa
    }
    logging("new app label: ${app.label}")
    app.updateLabel(appLa)
}
def logging(msg){
    def debug = settings.find{it.key == "enablelogging"}?.value
    if (debug) log.debug msg
    if(debug && atomicState.EnableDebugTime == null) atomicState.EnableDebugTime = now()

}
def descriptiontext(msg){
    def debug = settings.find{it.key == "enabledescriptiontext"}?.value
    if (debug) log.info msg

}
def disablelogging(){
    app.updateSetting("enablelogging",[value:"false",type:"bool"])
    log.warn "logging disabled!"
}
def getConnectString() {new groovy.json.JsonBuilder([uri: getFullLocalApiServerUrl(), type: atomicState.remoteType, token: atomicState.accessToken, connectionType: atomicState.localConnectionType]).toString().bytes.encodeBase64()}
def menuHeader(titleText){"<div style=\"width:102%;background-color:#1C2BB7;color:white;padding:4px;font-weight: bold;box-shadow: 1px 2px 2px #bababa;margin-left: -10px\">${titleText}</div>"}
