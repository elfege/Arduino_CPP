definition(
    name: "A.I. Thermostat Set Point Manager - Location Modes Edition",
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
    if(atomicState.paused)
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

        if(atomicState.paused == true)
        {
            atomicState.button_name = "resume"
            log.debug "button name is: $atomicState.button_name"
        }
        else 
        {
            atomicState.button_name = "pause"
            log.debug "button name is: $atomicState.button_name"
        }


        section("")
        {
            input "pause", "button", title: "$atomicState.button_name"
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
            input "setbaseline", "bool", title: "Set a base line", submitOnChange:true, defaultValue:false
            if(setbaseline)
            {
                paragraph "<h3>all default temperatures will be set to this value! This will delete all previous learnings!</h3>"
                input "baseline", "number", title: "enter a value between 1 and 108", range: "1..108"
                input "baseconfirm", "button", title: "confirm"
            }
            input "kill", "button", title: "kill current process", submitOnChange: true
        }
        section("")
        {
            input "showdata", "button", title: "Show the current data"
            if(atomicState.showdata)
            {
                paragraph "${showdata()}"
            }
        }

        section()
        {
            input "update", "button", title: "UPDATE"
            paragraph " ", width: 6
            input "resetdb", "button", title: "RESET", width: 6
            if(atomicState.confirm)
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
                atomicState.enablelogging = true
                runIn(240, disablelogging)
                log.info "logging enabled"
            }
            else 
            {
                atomicState.enablelogging = false
                log.warn "logging disabled"
            }
        }
    }
}

def installed() {
    logging("Installed with settings: ${settings}")
    runIn(5, createDb) // must be run only from this scope or user would lose all the acruded machine learning 
    initialize()
}
def updated() {

    logging("""updated with settings: ${settings}-""")
    atomicState.label = app.label
    unschedule()
    initialize()
}
def initialize(){
    //app.updateLabel("<font color = 'red'> ${app.label} </font>" )
    atomicState.kill = false
    logging("initializing ${app.label}...")

    unschedule()
    unsubscribe()


    subscribe(Thermostat, "heatingSetpoint", setPointHandler)
    subscribe(Thermostat, "coolingSetpoint", setPointHandler)
    //subscribe(dimmer, "level", setPointHandler)
    subscribe(outdoor, "temperature", temperatureHandler)
    subscribe(humidity,  "humidity", humidityHandler)

    //createDb() // for development only, comment out if not developing app or db will be reset on every update

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
            initialize()            
            break
        }
        case "update":
        atomicState.paused = false
        initialize()
        break
        case "resetdb":atomicState.confirm = true
        break
        case "confirm":atomicState.confirm = false
        createDb()
        break
        case "cancel":atomicState.confirm = false
        break
        case "baseconfirm":
        log.info "processing db. Please, be patient, this can take several minutes and may temporarily slow down your hub"
        if(baseline) {setBaseLine()}
        break
        case "showdata":
        atomicState.showdata = true
        runIn(30, cancelshowdata)
        case "kill":
        atomicState.kill = true
        log.warn "STATE KILL SET TO $atomicState.kill"
        pauseExecution(2000)
        break

    }
}

def showdata(){
    def alldb = alldb()
    int s = alldb.size()
    int i = 1
    def result = ""
    for(s!=0;i<s;i++)
    {
        def r = """<h3><b>DATABASE NUMBER $i: </h3></b>
${alldb[i]}
"""
        result += r
    }
    return result
}
def cancelshowdata(){
    atomicState.showdata = false
}

def setBaseLine()
{
    def alldb = alldb()

    log.trace """printing the database (for backup purpose if needed): 

$alldb

"""  
    i = 1 // alldb() index0 returns null in alldb() so we start from 1
    s = alldb.size()  // we have 7 db into which we need look for those values (outside temp, humidity), try each one until found
    for(s!=0;i<s&&!atomicState.paused;i++)
    {
        def sz = alldb[i].size()
        def a = 0 // index number in alldb[i] (containing the values)
        log.warn "updating DB$i... please, be patient"
        for(sz!=0;a<sz;a++)
        {
            alldb[i][a].remove(2)
            alldb[i][a] += baseline
            pauseExecution(1)
        }
    }
    logging("UPDATE FINISHED! Now printing the new database")
    pauseExecution(10)
    logging("$alldb")

}
def setPointHandler(evt){

    boolean restrictedMode = location.mode in restricted

    if(!restrictedMode)
    {
        if(!atomicState.paused)
        {
            atomicState.setpointHandlerRunning = true

            log.info( "************learning new desired temperature: $evt.value with outside temp: $outsideTemp and humidity: $humidity")

            def outsideTemp = outdoor.currentValue("temperature")
            def humidity = humidity.currentValue("humidity")
            def alldb = alldb()

            //              [outside temp, humidity, desired temp]                Desired temp is the value that will be written upon user's inputs


            // look for all the index[0] == outsideTemp +/-n  AND index[1] == current Humidity +/-n and modify index[2]   

            ref = [outsideTemp, humidity, evt.value]

            logging( "--------------learning WITH HUMIDITY")

            i = 1 // alldb() index0 returns nul in alldb() so we start from 1
            s = alldb.size()  // we have 7 db into which we need look for those values (outside temp, humidity), try each one until found
            for(s!=0;i<s&&!atomicState.paused;i++)
            {
                logging("SEARCHING DB$i:")
                int n = 1 // we start by adding +1 not +0
                int f = 5

                for(f!=0;n<f&&!atomicState.paused;n++)
                {
                    def sz = alldb[i].size()
                    def a = 0 // index number in alldb[i] (containing the values)


                    for(sz!=0;a<sz&&!atomicState.paused;a++)
                    {
                        def rec = alldb[i][a][0,1,2]
                        //logging ("$rec --->")

                        boolean A = alldb[i][a][0] == outsideTemp && alldb[i][a][1] == humidity // this will apply the new setpoint value to the current outsideTemp and humidity values

                        // now we test for close values within an amplitude of +/- n for both outsideTemp and humidity
                        boolean B = alldb[i][a][0] == outsideTemp && alldb[i][a][1] == humidity + n
                        boolean C = alldb[i][a][0] == outsideTemp && alldb[i][a][1] == humidity - n
                        boolean D = alldb[i][a][0] == outsideTemp + n && alldb[i][a][1] == humidity + n
                        boolean E = alldb[i][a][0] == outsideTemp - n && alldb[i][a][1] == humidity - n
                        boolean F = alldb[i][a][0] == outsideTemp + n && alldb[i][a][1] == humidity
                        boolean G = alldb[i][a][0] == outsideTemp - n && alldb[i][a][1] == humidity
                        boolean H = alldb[i][a][0] == outsideTemp + n && alldb[i][a][1] == humidity - n
                        boolean I = alldb[i][a][0] == outsideTemp - n && alldb[i][a][1] == humidity + n

                        if(A||B||C||D||E||F||G||H||I)
                        {
                            alldb[i][a].remove(2)
                            alldb[i][a] += evt.value 

                            pauseExecution(1)
                            def newRec = alldb[i][a][0,1,2]
                            logging("applied amplitude #$n replacing $rec with ${newRec}") 
                        }
                    }
                }
            }
            log.info "DONE"
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
    if(!atomicState.paused && !restrictedMode)
    {
        retrieve()
    }
}
def humidityHandler(evt)
{
    boolean inMode = location.mode in restricted
    logging("$evt.device returns ${evt.name}:${evt.value}")
    if(!atomicState.paused && !restrictedMode)
    {
        retrieve()
    }
}
def retrieve()
{
    def outsideTemp = outdoor.currentValue("temperature")
    def humidity = humidity.currentValue("humidity")
    def i = 1 // alldb() index ; 0 returns nul in alldb() so we start at 1
    def s = 7  // we have 7 db into which we need look for those values (outside temp, humidity), try each one until found
    boolean found = false
    def alldb = alldb()

    // we want to find the item in db that contains this evt.value outside temp and current humidity so we can extrat index 2, that is the corresponding desired temp
    def ref = [outsideTemp, humidity] // values to look for
    def desired = null

    for(s!=0;i<s&&!found;i++)
    {

        def sz = alldb[i].size()
        def a = 0 // index number in alldb[i] (containing the values)


        for(sz!=0;a<sz && !found;a++)
        {
            if(alldb[i][a][0,1] == ref[0,1])
            {
                found = true
                logging("FOUND MATCH in DB$i----index $a ----------${ref[0,1]} == ${alldb[i][a][0,1]}---------------------------------------------------------")
                desired = alldb[i][a][2]

                break;
            }
        }
    }

    logging("new desired temperature for current weather is: ${desired}°F")
    if(dimmer.currentValue("level") != desired)
    {
        dimmer.setLevel(desired)
        logging("$dimmer set to $desired")
    }
    else 
    {
        logging("$dimmer is already set to $desired")
    }

}

def logging(message)
{
    //atomicState.enablelogging = true
    if(atomicState.enablelogging) log.debug message

}
def disablelogging()
{
    atomicState.enablelogging = false
}
def createDb()
{
    log.info "CREATING DATABASE... this will happen only at first install and may take some ressources for a little while. Temporary slow downs to be expected on your system"

    createDb1()


    logging("""
-----------------------
A.I. map installed
-----------------------
-""")

}




def createDb1()
{
    // [outside temp, humidity, desired temp] Desired temp is the valu that will be written upon user's inputs
    logging("installing db1")

    int i = 0
    int j = 0
    int k = 75 // base line variable
    def db = [[1,1,1]]
    int safety = 0
    kill = false


    while(1/*db[db.size()] != [108,100,k] &&*/ )
    {

        // logging("*****************db is ${db} db[0] is ${db[0]} db[$i] size is ${db[i].size()} db[i][0] = ${db[i][0]}")

        //build index 0
        //db[i].remove(0)
        /*
logging("db[i] was ${db[i]}")
db[i].remove(2)    
db[i].remove(1)      
db[i].remove(0)

i = 0
while(i < 108)
{
log.info"i---------------$i"
db[i][0] += 1

i += 1
}
j = 0
while(j < 100)
{
log.info"j---------------$j"
db[i][1] += 1

j += 1
}
db[i][2] = k



pauseExecution(5)
logging("""
db[i][0] is now ${db[i][0]}
db[i][1] is now ${db[i][1]}
db[i][2] is now ${db[i][2]}
db[i] -----------> ${db[i]}
""")
*/
        safety += 1
        if(safety>10)
        {
            log.warn "FAILLURE"
            return
        }

        pauseExecution(1000)
        log.warn "# $safety  atomicState.kill = ${atomicState.kill}"
                   

        if(atomicState.kill == true)
        {
            return   
        }

    }

    logging("$db")


    /*[0,92,75],[1,92,75],[2,92,75],[3,92,75],[4,92,75],[5,92,75],[6,92,75],[7,92,75],[8,92,75],[9,92,75],[10,92,75],[11,92,75],[12,92,75],[13,92,75],[14,92,75],[15,92,75],[16,92,75],[17,92,75],[18,92,75],[19,92,75],[20,92,75],[21,92,75],[22,92,75],[23,92,75],[24,92,75],[25,92,75],[26,92,75],[27,92,75],[28,92,75],[29,92,75],[30,92,75],[31,92,74],[32,92,74],[33,92,74],[34,92,74],[35,92,74],[36,92,73],[37,92,73],[38,92,73],[39,92,73],[40,92,73],[41,92,73],[42,92,73],[43,92,73],[44,92,73],[45,92,73],[46,92,73],[47,92,73],[48,92,73],[49,92,73],[50,92,73],[51,92,73],[52,92,73],[53,92,73],[54,92,73],[55,92,73],[56,92,73],[57,92,73],[58,92,73],[59,92,73],[60,92,73],[61,92,73],[62,92,73],[63,92,72],[64,92,72],[65,92,71],[66,92,71],[67,92,71],[68,92,72],[69,92,72],[70,92,72],[71,92,72],[72,92,72],[73,92,72],[74,92,72],[75,92,72],[76,92,72],[77,92,73],[78,92,73],[79,92,73],[80,92,73],[81,92,73],[82,92,73],[83,92,73],[84,92,73],[85,92,73],[86,92,73],[87,92,73],[88,92,73],[89,92,73],[90,92,73],[91,92,73],[92,92,73],[93,92,74],[94,92,74],[95,92,74],[96,92,74],[97,92,74],[98,92,74],[99,92,74],[100,92,74],[101,92,75],[102,92,75],[103,92,75],[104,92,75],[105,92,75],[106,92,75],[107,92,75],[108,92,75],
[0,93,75],[1,93,75],[2,93,75],[3,93,75],[4,93,75],[5,93,75],[6,93,75],[7,93,75],[8,93,75],[9,93,75],[10,93,75],[11,93,75],[12,93,75],[13,93,75],[14,93,75],[15,93,75],[16,93,75],[17,93,75],[18,93,75],[19,93,75],[20,93,75],[21,93,75],[22,93,75],[23,93,75],[24,93,75],[25,93,75],[26,93,75],[27,93,75],[28,93,75],[29,93,75],[30,93,75],[31,93,74],[32,93,74],[33,93,74],[34,93,74],[35,93,74],[36,93,73],[37,93,73],[38,93,73],[39,93,73],[40,93,73],[41,93,73],[42,93,73],[43,93,73],[44,93,73],[45,93,73],[46,93,73],[47,93,73],[48,93,73],[49,93,73],[50,93,73],[51,93,73],[52,93,73],[53,93,73],[54,93,73],[55,93,73],[56,93,73],[57,93,73],[58,93,73],[59,93,73],[60,93,73],[61,93,73],[62,93,73],[63,93,72],[64,93,72],[65,93,71],[66,93,71],[67,93,71],[68,93,72],[69,93,72],[70,93,72],[71,93,72],[72,93,72],[73,93,72],[74,93,72],[75,93,72],[76,93,72],[77,93,73],[78,93,73],[79,93,73],[80,93,73],[81,93,73],[82,93,73],[83,93,73],[84,93,73],[85,93,73],[86,93,73],[87,93,73],[88,93,73],[89,93,73],[90,93,73],[91,93,73],[92,93,73],[93,93,74],[94,93,74],[95,93,74],[96,93,74],[97,93,74],[98,93,74],[99,93,74],[100,93,74],[101,93,75],[102,93,75],[103,93,75],[104,93,75],[105,93,75],[106,93,75],[107,93,75],[108,93,75],
[0,94,75],[1,94,75],[2,94,75],[3,94,75],[4,94,75],[5,94,75],[6,94,75],[7,94,75],[8,94,75],[9,94,75],[10,94,75],[11,94,75],[12,94,75],[13,94,75],[14,94,75],[15,94,75],[16,94,75],[17,94,75],[18,94,75],[19,94,75],[20,94,75],[21,94,75],[22,94,75],[23,94,75],[24,94,75],[25,94,75],[26,94,75],[27,94,75],[28,94,75],[29,94,75],[30,94,75],[31,94,74],[32,94,74],[33,94,74],[34,94,74],[35,94,74],[36,94,73],[37,94,73],[38,94,73],[39,94,73],[40,94,73],[41,94,73],[42,94,73],[43,94,73],[44,94,73],[45,94,73],[46,94,73],[47,94,73],[48,94,73],[49,94,73],[50,94,73],[51,94,73],[52,94,73],[53,94,73],[54,94,73],[55,94,73],[56,94,73],[57,94,73],[58,94,73],[59,94,73],[60,94,73],[61,94,73],[62,94,73],[63,94,72],[64,94,72],[65,94,71],[66,94,71],[67,94,71],[68,94,72],[69,94,72],[70,94,72],[71,94,72],[72,94,72],[73,94,72],[74,94,72],[75,94,72],[76,94,72],[77,94,73],[78,94,73],[79,94,73],[80,94,73],[81,94,73],[82,94,73],[83,94,73],[84,94,73],[85,94,73],[86,94,73],[87,94,73],[88,94,73],[89,94,73],[90,94,73],[91,94,73],[92,94,73],[93,94,74],[94,94,74],[95,94,74],[96,94,74],[97,94,74],[98,94,74],[99,94,74],[100,94,74],[101,94,75],[102,94,75],[103,94,75],[104,94,75],[105,94,75],[106,94,75],[107,94,75],[108,94,75],
[0,95,75],[1,95,75],[2,95,75],[3,95,75],[4,95,75],[5,95,75],[6,95,75],[7,95,75],[8,95,75],[9,95,75],[10,95,75],[11,95,75],[12,95,75],[13,95,75],[14,95,75],[15,95,75],[16,95,75],[17,95,75],[18,95,75],[19,95,75],[20,95,75],[21,95,75],[22,95,75],[23,95,75],[24,95,75],[25,95,75],[26,95,75],[27,95,75],[28,95,75],[29,95,75],[30,95,75],[31,95,74],[32,95,74],[33,95,74],[34,95,74],[35,95,74],[36,95,73],[37,95,73],[38,95,73],[39,95,73],[40,95,73],[41,95,73],[42,95,73],[43,95,73],[44,95,73],[45,95,73],[46,95,73],[47,95,73],[48,95,73],[49,95,73],[50,95,73],[51,95,73],[52,95,73],[53,95,73],[54,95,73],[55,95,73],[56,95,73],[57,95,73],[58,95,73],[59,95,73],[60,95,73],[61,95,73],[62,95,73],[63,95,72],[64,95,72],[65,95,71],[66,95,71],[67,95,71],[68,95,72],[69,95,72],[70,95,72],[71,95,72],[72,95,72],[73,95,72],[74,95,72],[75,95,72],[76,95,72],[77,95,73],[78,95,73],[79,95,73],[80,95,73],[81,95,73],[82,95,73],[83,95,73],[84,95,73],[85,95,73],[86,95,73],[87,95,73],[88,95,73],[89,95,73],[90,95,73],[91,95,73],[92,95,73],[93,95,74],[94,95,74],[95,95,74],[96,95,74],[97,95,74],[98,95,74],[99,95,74],[100,95,74],[101,95,75],[102,95,75],[103,95,75],[104,95,75],[105,95,75],[106,95,75],[107,95,75],[108,95,75],
[0,96,75],[1,96,75],[2,96,75],[3,96,75],[4,96,75],[5,96,75],[6,96,75],[7,96,75],[8,96,75],[9,96,75],[10,96,75],[11,96,75],[12,96,75],[13,96,75],[14,96,75],[15,96,75],[16,96,75],[17,96,75],[18,96,75],[19,96,75],[20,96,75],[21,96,75],[22,96,75],[23,96,75],[24,96,75],[25,96,75],[26,96,75],[27,96,75],[28,96,75],[29,96,75],[30,96,75],[31,96,74],[32,96,74],[33,96,74],[34,96,74],[35,96,74],[36,96,73],[37,96,73],[38,96,73],[39,96,73],[40,96,73],[41,96,73],[42,96,73],[43,96,73],[44,96,73],[45,96,73],[46,96,73],[47,96,73],[48,96,73],[49,96,73],[50,96,73],[51,96,73],[52,96,73],[53,96,73],[54,96,73],[55,96,73],[56,96,73],[57,96,73],[58,96,73],[59,96,73],[60,96,73],[61,96,73],[62,96,73],[63,96,72],[64,96,72],[65,96,71],[66,96,71],[67,96,71],[68,96,72],[69,96,72],[70,96,72],[71,96,72],[72,96,72],[73,96,72],[74,96,72],[75,96,72],[76,96,72],[77,96,73],[78,96,73],[79,96,73],[80,96,73],[81,96,73],[82,96,73],[83,96,73],[84,96,73],[85,96,73],[86,96,73],[87,96,73],[88,96,73],[89,96,73],[90,96,73],[91,96,73],[92,96,73],[93,96,74],[94,96,74],[95,96,74],[96,96,74],[97,96,74],[98,96,74],[99,96,74],[100,96,74],[101,96,75],[102,96,75],[103,96,75],[104,96,75],[105,96,75],[106,96,75],[107,96,75],[108,96,75],
[0,97,75],[1,97,75],[2,97,75],[3,97,75],[4,97,75],[5,97,75],[6,97,75],[7,97,75],[8,97,75],[9,97,75],[10,97,75],[11,97,75],[12,97,75],[13,97,75],[14,97,75],[15,97,75],[16,97,75],[17,97,75],[18,97,75],[19,97,75],[20,97,75],[21,97,75],[22,97,75],[23,97,75],[24,97,75],[25,97,75],[26,97,75],[27,97,75],[28,97,75],[29,97,75],[30,97,75],[31,97,74],[32,97,74],[33,97,74],[34,97,74],[35,97,74],[36,97,73],[37,97,73],[38,97,73],[39,97,73],[40,97,73],[41,97,73],[42,97,73],[43,97,73],[44,97,73],[45,97,73],[46,97,73],[47,97,73],[48,97,73],[49,97,73],[50,97,73],[51,97,73],[52,97,73],[53,97,73],[54,97,73],[55,97,73],[56,97,73],[57,97,73],[58,97,73],[59,97,73],[60,97,73],[61,97,73],[62,97,73],[63,97,72],[64,97,72],[65,97,71],[66,97,71],[67,97,71],[68,97,72],[69,97,72],[70,97,72],[71,97,72],[72,97,72],[73,97,72],[74,97,72],[75,97,72],[76,97,72],[77,97,73],[78,97,73],[79,97,73],[80,97,73],[81,97,73],[82,97,73],[83,97,73],[84,97,73],[85,97,73],[86,97,73],[87,97,73],[88,97,73],[89,97,73],[90,97,73],[91,97,73],[92,97,73],[93,97,74],[94,97,74],[95,97,74],[96,97,74],[97,97,74],[98,97,74],[99,97,74],[100,97,74],[101,97,75],[102,97,75],[103,97,75],[104,97,75],[105,97,75],[106,97,75],[107,97,75],[108,97,75],
[0,98,75],[1,98,75],[2,98,75],[3,98,75],[4,98,75],[5,98,75],[6,98,75],[7,98,75],[8,98,75],[9,98,75],[10,98,75],[11,98,75],[12,98,75],[13,98,75],[14,98,75],[15,98,75],[16,98,75],[17,98,75],[18,98,75],[19,98,75],[20,98,75],[21,98,75],[22,98,75],[23,98,75],[24,98,75],[25,98,75],[26,98,75],[27,98,75],[28,98,75],[29,98,75],[30,98,75],[31,98,74],[32,98,74],[33,98,74],[34,98,74],[35,98,74],[36,98,73],[37,98,73],[38,98,73],[39,98,73],[40,98,73],[41,98,73],[42,98,73],[43,98,73],[44,98,73],[45,98,73],[46,98,73],[47,98,73],[48,98,73],[49,98,73],[50,98,73],[51,98,73],[52,98,73],[53,98,73],[54,98,73],[55,98,73],[56,98,73],[57,98,73],[58,98,73],[59,98,73],[60,98,73],[61,98,73],[62,98,73],[63,98,72],[64,98,72],[65,98,71],[66,98,71],[67,98,71],[68,98,72],[69,98,72],[70,98,72],[71,98,72],[72,98,72],[73,98,72],[74,98,72],[75,98,72],[76,98,72],[77,98,73],[78,98,73],[79,98,73],[80,98,73],[81,98,73],[82,98,73],[83,98,73],[84,98,73],[85,98,73],[86,98,73],[87,98,73],[88,98,73],[89,98,73],[90,98,73],[91,98,73],[92,98,73],[93,98,74],[94,98,74],[95,98,74],[96,98,74],[97,98,74],[98,98,74],[99,98,74],[100,98,74],[101,98,75],[102,98,75],[103,98,75],[104,98,75],[105,98,75],[106,98,75],[107,98,75],[108,98,75],
[0,99,75],[1,99,75],[2,99,75],[3,99,75],[4,99,75],[5,99,75],[6,99,75],[7,99,75],[8,99,75],[9,99,75],[10,99,75],[11,99,75],[12,99,75],[13,99,75],[14,99,75],[15,99,75],[16,99,75],[17,99,75],[18,99,75],[19,99,75],[20,99,75],[21,99,75],[22,99,75],[23,99,75],[24,99,75],[25,99,75],[26,99,75],[27,99,75],[28,99,75],[29,99,75],[30,99,75],[31,99,74],[32,99,74],[33,99,74],[34,99,74],[35,99,74],[36,99,73],[37,99,73],[38,99,73],[39,99,73],[40,99,73],[41,99,73],[42,99,73],[43,99,73],[44,99,73],[45,99,73],[46,99,73],[47,99,73],[48,99,73],[49,99,73],[50,99,73],[51,99,73],[52,99,73],[53,99,73],[54,99,73],[55,99,73],[56,99,73],[57,99,73],[58,99,73],[59,99,73],[60,99,73],[61,99,73],[62,99,73],[63,99,72],[64,99,72],[65,99,71],[66,99,71],[67,99,71],[68,99,72],[69,99,72],[70,99,72],[71,99,72],[72,99,72],[73,99,72],[74,99,72],[75,99,72],[76,99,72],[77,99,73],[78,99,73],[79,99,73],[80,99,73],[81,99,73],[82,99,73],[83,99,73],[84,99,73],[85,99,73],[86,99,73],[87,99,73],[88,99,73],[89,99,73],[90,99,73],[91,99,73],[92,99,73],[93,99,74],[94,99,74],[95,99,74],[96,99,74],[97,99,74],[98,99,74],[99,99,74],[100,99,74],[101,99,75],[102,99,75],[103,99,75],[104,99,75],[105,99,75],[106,99,75],[107,99,75],[108,99,75],
[0,100,75],[1,100,75],[2,100,75],[3,100,75],[4,100,75],[5,100,75],[6,100,75],[7,100,75],[8,100,75],[9,100,75],[10,100,75],[11,100,75],[12,100,75],[13,100,75],[14,100,75],[15,100,75],[16,100,75],[17,100,75],[18,100,75],[19,100,75],[20,100,75],[21,100,75],[22,100,75],[23,100,75],[24,100,75],[25,100,75],[26,100,75],[27,100,75],[28,100,75],[29,100,75],[30,100,75],[31,100,74],[32,100,74],[33,100,74],[34,100,74],[35,100,74],[36,100,73],[37,100,73],[38,100,73],[39,100,73],[40,100,73],[41,100,73],[42,100,73],[43,100,73],[44,100,73],[45,100,73],[46,100,73],[47,100,73],[48,100,73],[49,100,73],[50,100,73],[51,100,73],[52,100,73],[53,100,73],[54,100,73],[55,100,73],[56,100,73],[57,100,73],[58,100,73],[59,100,73],[60,100,73],[61,100,73],[62,100,73],[63,100,72],[64,100,72],[65,100,71],[66,100,71],[67,100,71],[68,100,72],[69,100,72],[70,100,72],[71,100,72],[72,100,72],[73,100,72],[74,100,72],[75,100,72],[76,100,72],[77,100,73],[78,100,73],[79,100,73],[80,100,73],[81,100,73],[82,100,73],[83,100,73],[84,100,73],[85,100,73],[86,100,73],[87,100,73],[88,100,73],[89,100,73],[90,100,73],[91,100,73],[92,100,73],[93,100,74],[94,100,74],[95,100,74],[96,100,74],[97,100,74],[98,100,74],[99,100,74],
[100,100,74],[101,100,75],[102,100,75],[103,100,75],[104,100,75],[105,100,75],[106,100,75],[107,100,75],[108,100,75]       
*/


}


def alldb()
{
    return [null, atomicState.tempAImap_1,atomicState.tempAImap_2,atomicState.tempAImap_3,atomicState.tempAImap_4,atomicState.tempAImap_5,atomicState.tempAImap_6,atomicState.tempAImap_7]  
}
