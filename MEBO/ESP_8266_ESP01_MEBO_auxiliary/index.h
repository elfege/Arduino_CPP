const char MAIN_page[] PROGMEM = R"=====(
<html>
<style>
    :root {
        --buttonWidth: 150px;
        --buttonHeight: 50px;
    }

    h4 {
        text-align: center;
    }

    section {
        /* border: 1px solid lightblue; */
        margin: auto;
        width: 100%;
    }

    iframe {
        border: 1px solid black;
        width: 100%;
        height: 400px;
    }

    button {
        border: 5px solid lightgray;
        background-color: rgb(125, 156, 156);
        margin-bottom: 5px;
        width: var(--buttonWidth);
        height: var(--buttonHeight);
        font-weight: bolder;
        position: relative;


    }

    button:hover {
        color: white;
    }

    #stop {
        background-color: red;
    }

    #pause {
        background-color: pink;
    }

    #serial {
        border: 1px solid green;
        background-color: bisque;
    }

    span {
        color: black;
    }

    .sensorsDiv {
        border: 1px solid rgb(205, 189, 189);
        position: relative;
        height: 100%;
        width: 100%;
        margin: auto;
        text-align: center;
    }

    .sensors {
        /* border: 1px solid rgb(202, 196, 196); */
        position: relative;
        width: var(--buttonWidth);
        height: var(--buttonHeight);
        margin: 5px;
        font-weight: bolder;
        text-align: center;
    }



    .buttonDivs {
        /* border: 1px solid rgb(40, 35, 35); */
        display: block;

        position: relative;
        top: 0%;
        margin: auto;
        text-align: center;
        margin: 5px;
    }

    .insideDiv {
        margin: 2px;
        display: inline-block;
    }

    #feed {
        background-color: blue;
        color: white;
    }

    #vertical {
        background-color: bisque;
    }

    #reset {
        background-color: black;
        color: white;
    }

    #verticalDown,
    #verticalUp {
        background-color: blueviolet;
    }

    table {
        position:relative;
        margin:auto;
        width:50%;
    }
    tr,
    .maintds {
        /*border: 1px solid gray;*/
        display: -webkit-box;
        display: -moz-box;
        display: -ms-flexbox;
        display: -webkit-flex;
        display: flex;
    }

    #system {
        display: -webkit-box;
        display: -moz-box;
        display: -ms-flexbox;
        display: -webkit-flex;
        display: flex;
    }

    #verticalupsensor {
        background-color: green;
    }
</style>

<head>
    <meta charset="UTF-8" />
    <meta name"cache-control" content="public, max-age=0">
    <title>MEBO POWER SWITCH</title>

</head>

<body>

    <section>
        <table>
            <tr>
                <center>
                    <h1 style="color:blue;">MEBO POWER SWITCH</h1>
                    <h6 id="localip"></h6>
                    <h6 id="mac"></h6>
                </center>
            </tr>
            <tr>
                <td class="maintds">
                    <div class="buttonDivs">
                        <button id="onoff">STATE</button>
                        <button id="OFF">OFF</button>
                        <button id="ON">ON</button>
                </td>
                <td id="system">
                    <div class="buttonDivs">
                        <button id="refresh">REFRESH</button>
                        <button id="reset">RESET</button>
                        <button id="debug">DEBUG</button>
                        <button id="term">TERMINAL</button>
                        <button id="toggleIntervals">STOP SCRIPTS</button>
                    </div>
                </td>
            </tr>
            <tr>
                <td colspan="3" width="100%">
                    <h4 id="serial">Serial Output</h4>
                    <iframe id="terminal" hidden src=""></iframe>
                </td>
            </tr>
        </table>
    </section>

</body>

<script src="https://code.jquery.com/jquery-3.6.0.js"></script>
<script src="https://code.jquery.com/ui/1.13.2/jquery-ui.js"></script>
<script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
<script src="https://unpkg.com/jquery"></script>

<script>
    let localIp;
    let Logs = false;


    $("button").on("click", showTermHandler)
    $("#serial").on("click", showTermHandler)

    function showTermHandler(e) {
        e.preventDefault();
        debug("e.target.getAttribute('id'): ", e.target.getAttribute("id"))
        const id = e.target.getAttribute("id");
        debug("click event:", id, `id in ["term", "serial"] ? => , ${id in ["term", "serial"]}`)
        if (["term", "serial"].find(val => val === id)) {

            const terminal = $("#terminal");
            terminal.toggle();
            console.log("opening term at: " + localIp)
            terminal.attr("src", `http://${localIp}/term.html`);
        }
        else {
            mainFunc(e.target.id)
        }
    }

    let scriptOn = false;
    async function mainFunc(id) {
        console.log("mainFunc(", id, ")")

        if (id === "debug") {
            Logs = !Logs;
        }

        const response = await axios({
            url: `/${id}`,
            method: "GET",
        });

        const data = response.data;
        console.log("response: ", data)

        if (id === "toggleIntervals") {
            if (!scriptOn) {
                $("#toggleIntervals").text("STOP SCRIPTS")
                $("#toggleIntervals").css({
                    "background-color": "blue",
                    "color": "white"
                })
                resumeIntervals()

            }
            else if (scriptOn) {
                $("#toggleIntervals").text("RESUME SCRIPTS")
                $("#toggleIntervals").css({
                    "background-color": "red",
                    "color": "white"
                })
                clearAllIntervals()


            }
            return
        }

       

    }

    const ids = []
    const elements = $("*")
    const array = Array.from(elements)
    array.forEach((val) => {
        if ($(val).attr("id") !== undefined) ids.push($(val).attr("id"))
    });

    async function checkStates() {

        const response = await axios({
            url: "/getstates",
            method: "GET",
        });

        const data = response.data;
        // debug("checkStates data:" + data);

        const arr = data.split(",")
        const newArr = []
        arr.forEach((val) => {
            newArr.push(val.replace(" ", ":").split(":"))
        })
        const Obj = Object.fromEntries(newArr)
        debug("entries:" + Obj)
        for (let a of Object.keys(Obj)) {
            // debug(a + ":" + Obj[a])
            if (a.includes("slider")) {
                $(`#${a}`).val(Obj[a]);
                $(`#sliderValue`).text(Obj[a])
            }
            else {
                $(`#${a}`).text(Obj[a]);               
            }
        }
    }


    function debug(msg) {
        if (Logs) console.log(msg)
    }


    async function getLocalIp() {
        const response = await axios({
            url: "/getip",
            method: "GET",
        });
        console.log("ip response: ", response.data)
        localIp = response.data;
        $("#localip").text(localIp);
    }
    async function getMac() {
        const response = await axios({
            url: "/getmac",
            method: "GET",
        });
        console.log("mac response: ", response.data)
        mac = response.data;
        $("#mac").text(mac);
    }
    getMac()


    document.addEventListener("DOMContentLoaded", () => {
        sliderEventListeners();
        getLocalIp();

    })

    function sliderEventListeners() {
        const Sliders = $("input")
        const sliderValues = $(".sliderValue")

        const SlidersArray = Array.from(Sliders)
        const SlidersValuesArr = Array.from(sliderValues)

        SlidersArray.forEach((slider, index) => {

            console.log("creating evt listner for", $(slider).attr("class"))
            console.log("slider.class", $(slider).attr("class"))

            $(slider).on("input", (e) => {
                const id = e.target.id
                console.log("input event ", id)
                const xhr = new XMLHttpRequest();
                const url = id + "=" + $(slider).val();
                console.log("sliderValue = ", $(slider).val())
                const name = id
                $(SlidersValuesArr[index]).text(name + "'s speed: " + $(slider).val())

                const response = axios({
                    url: url,
                    method: "GET",
                });

                // xhr.onreadystatechange = function () {
                // if (this.readyState == 4 && this.status == 200) {
                // console.log("slider response: ", xhr.responseText);
                // $(slider).val(response.data)
                $(e.target.id).val(response.data)
                $(SlidersValuesArr[index]).text(response.data)
                // }
                // };
                // xhr.open("GET", url, true);
                // xhr.send();
            })
        })
    }

    let checkStatesInterval = null;
    let ipRetrieveInterval = null;
    function resumeIntervals() {
        scriptOn = true;
        $("#toggleIntervals").css({
            "background-color": "blue",
            "color": "white"
        })
        console.log("resuming intervals");
        checkStatesInterval = setInterval(checkStates, 500);
        ipRetrieveInterval = setInterval(getLocalIp, 60 * 60 * 1000);
    }

    resumeIntervals();

    //setTimeout(clearAllIntervals, 60 * 5 * 1000)

    function clearAllIntervals() {
        scriptOn = false;
        console.log("clearing intervals")
        clearInterval(checkStatesInterval);
    }


</script>


</html>
)=====";