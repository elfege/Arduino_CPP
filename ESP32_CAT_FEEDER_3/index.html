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
    /* border: 1px solid black; */
    width: 100%;
    height: 400px;
  }

  button {
    border: 1px solid lightgray !important;
    /* background-color: rgb(125, 156, 156); */
    /* margin-bottom: 5px; */
    width: var(--buttonWidth);
    height: var(--buttonHeight);
    font-weight: bolder !important;
    position: relative;
    border-radius: 5px !important;


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
    /* border: 1px solid green; */
    background-color: bisque;
  }

  span {
    color: rgb(0, 0, 0);
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

  #OTA {
    background-color: rgb(45, 31, 70);
    color: white;
  }

  #verticaldown,
  #verticalup {
    background-color: rgb(184, 180, 188);
    color: black !important;

  }

  .sensors {
    color: blue;
    position: relative;
    text-align: left !important;
  }

  #fillstate {
    color: red;
  }
</style>

<head>
  <meta charset="UTF-8" />
  <meta name"cache-control"="name" cache-control""="cache-control""" content="public, max-age=0">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/4.0.0/css/bootstrap.min.css" rel="stylesheet" />
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css" rel="stylesheet"
    integrity="sha384-GLhlTQ8iRABdZLl6O3oVMWSktQOp6b7In1Zl3/Jr59b6EGGoI1aFkw7cmDA6j6gD" crossorigin="anonymous">

  <script src="https://unpkg.com/@popperjs/core@2"></script>
  <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap-icons@1.7.2/font/bootstrap-icons.css'>

  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/js/bootstrap.bundle.min.js"
    integrity="sha384-w76AqPfDkMBDXo30jS1Sgez6pr3x5MlQ1ZAGC+nuZB+EYdgRZgiwxhTBTkF7CXvN"
    crossorigin="anonymous"></script>
  <title>CAT FEEDER 3.0 - Elfege Leylavergne - Smart Home Automation USA - Software Engineering - Robotics</title>

</head>

<body>

  <div class="container">
    <div class="row">
      <div class="col-xs-12">
        <center>
          <h1 style="color:blue;">CAT FEEDER 3</h1>
          <h6 id="localip"></h6>
          <h6 id="macAddress"></h6>
        </center>
      </div>
    </div>
    <div class="row">
      <div class="col-lg-2">
        <div class="container">
          <h1 class="row">STATUS</h1>
          <h4 class="row sensors" id="fillstate">fillstate</h4>
          <h4 class="row sensors" id="irval1">irval1<span>decomissioned</span></h4>
          <h4 class="row sensors" id="irval2">irval2</h4>
          <div class="row"></div>
          <p>
          <h4 class="row sensors" id="verticalstate">verticalstate</h4>
          <h4 class="row sensors" id="canstate">canstate</h4>
          <!-- deformation detected by impedance sensor -->
          <h4 class="row sensors" id="deformation">deformation</h4>
        </div>

      </div>

      <div class="col">

        <button class="btn btn-outline-primary m-1" id="feed">FEED</button>
        <button class="btn btn-outline-primary m-1" id="stop">STOP</button>
        <button class="btn btn-outline-primary m-1" id="pause">PAUSE</button>
        <button class="btn btn-outline-primary m-1" id="clean">CLEAN</button>
        <button class="btn btn-outline-primary m-1" id="freecanlid">LID DETACH</button>
      </div>
      <div class="col">
        <button class="btn btn-outline-warning m-1" id="verticaldown">VERTICAL DOWN</button>
        <button class="btn btn-outline-warning m-1" id="verticalup">VERTICAL UP</button>
        <button class="btn btn-outline-primary m-1" id="verticalstepdown">STEP DOWN</button>
        <button class="btn btn-outline-primary m-1" id="verticalstepup">STEP UP</button>
      </div>

      <div class="col">
        <button class="btn btn-outline-primary m-1" id="push">PUSH ARM</button>
        <button class="btn btn-outline-primary m-1" id="pull">PULL ARM</button>
        <button class="btn btn-outline-primary m-1" id="load">LOAD A CAN</button>
        <button class="btn btn-outline-primary m-1" id="lock">LOCK</button>
        <button class="btn btn-outline-primary m-1" id="force_unlock">FORCE UNLOCK</button>
        <button class="btn btn-outline-primary m-1" id="force_lock">FORCE LOCK</button>
        <button class="btn btn-outline-primary m-1" id="open">OPEN CAN</button>
        <button class="btn btn-outline-primary m-1" id="openfree">NO ROT SENSOR</button>
        <div class="insideDiv" id="openerspeed">
          <span id="sliderValueOpener" class="sliderValue">CAN OPENER'S SPEED</span>
          <input id="sliderOpener" class="slider" type="range" min="0" max="100">
        </div>
      </div>
      <div class="col">
        <button class="btn btn-outline-primary m-1" id="extract">EXTRACTION</button>
        <button class="btn btn-outline-primary m-1" id="smallpush">SMALL PUSH</button>
        <button class="btn btn-outline-primary m-1" id="smallpull">SMALL PULL</button>
      </div>

      <div class="col">
        <span id="sliderValueCosinus" for="sliderCosinus" class="sliderValue">ANGLE PUSHER'S SPEED</span>
        <input id="sliderCosinus" class="slider" type="range" min="0" max="100">

        <span>ANGLE PUSH - FINE TUNE</span>

        <button class="btn btn-outline-primary m-1" id="cosinusplus">+</button>
        <button class="btn btn-outline-primary m-1" id="cosinusminus">-</button>
        <button class="btn btn-outline-primary m-1" id="recordcosmax">record</button>
        <button class="btn btn-outline-primary m-1" id="cosinuspush">ANGLE PUSH</button>
        <button class="btn btn-outline-primary m-1" id="cosinuspull">ANGLE PULL</button>
      </div>

      <div class="col">
        <button class="btn btn-outline-primary m-1" id="reset">RESET</button>
        <button class="btn btn-outline-primary m-1" id="logs">DEBUG</button>
        <button class="btn btn-outline-primary m-1" id="term">TERMINAL</button>
        <button class="btn btn-outline-primary m-1" id="toggleIntervals">STOP SCRIPTS</button>
        <button class="btn btn-outline-primary m-1" id="OTA">OTA</button>


      </div>

    </div>
    <div class="row">
      <div colspan="col-lg-6 w-100">
        <h4 id="serial">Serial Output</h4>
        <iframe id="terminal" hidden="hidden" src="http://192.168.10.241/term.html"></iframe>

      </div>
    </div>
  </div>

</body>

<script src="https://code.jquery.com/jquery-3.6.0.js"></script>
<script src="https://code.jquery.com/ui/1.13.2/jquery-ui.js"></script>
<script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
<script src="https://unpkg.com/jquery"></script>

<script>
  let localIp;
  let Logs = false;

  jQuery(() => {
    $("button").on("click", clickHandler)
    $("#serial").on("click", clickHandler)

    let intervalID;

    $("#verticalstepdown").on("mousedown", () => {
      intervalID = setInterval(() => {
        mainFunc("verticalstepdown");
      }, 100); // Change the interval time as per your requirement
    });

    $("#verticalstepdown").on("mouseup", () => {
      clearInterval(intervalID);
    });

    $("#verticalstepup").on("mousedown", () => {
      intervalID = setInterval(() => {
        mainFunc("verticalstepup");
      }, 100); // Change the interval time as per your requirement
    });

    $("#verticalstepup").on("mouseup", () => {
      clearInterval(intervalID);
    });

  })

  function clickHandler(e) {
    e.preventDefault();
    debug("e.target.getAttribute('id'): ", e.target.getAttribute("id"))
    const id = e
      .target
      .getAttribute("id");
    debug("click event:", id, `id in ["term", "serial"] ? => , ${id in ["term", "serial"]}`)
    if (["term", "serial"].find(val => val === id)) {

      const terminal = $("#terminal");
      // console.log("terminal.prop('hidden') ==== ", terminal.prop("hidden") === true)
      if (terminal.prop("hidden") === false) {
        terminal.prop("hidden", true);
      }
      else {
        terminal.removeAttr("hidden");
      }
      console.log("opening term at: " + localIp)
      terminal.attr("src", `http://${localIp}/term.html`);
    } else {
      mainFunc(e.target.id)
    }
  }

  let scriptOn = false;

  const mainFunc = async (id) => {
    console.log(`mainFunc(${id})`);

    try {
      if (id === "logs") {
        Logs = !Logs;
      }

      if (id === "toggleIntervals") {
        if (!scriptOn) {
          $("#toggleIntervals").text("STOP SCRIPTS");
          $("#toggleIntervals").css({ "background-color": "blue", "color": "white" });
          resumeIntervals();
        } else {
          $("#toggleIntervals").text("RESUME SCRIPTS");
          $("#toggleIntervals").css({ "background-color": "red", "color": "white" });
          clearAllIntervals();
        }
        return;
      }

      if (id === "OTA") {
        const element = $(`#${id}`);
        if (confirm("OTA READY. Page will be unresponsive for at least 1 minute. Are you sure you want the controller to be in OTA mode?")) {
          console.log("Activating OTA MODE");

          await axios.get("/OTA")
          console.log(response.data)
          element.text(response.data);

          response = await axios.get("/awaitOTA", {
            timeout: 62000  // http response to be sent from mainHandler() after 60 seconds.  
          })
          if (response.data == "OTA") {
            element.text(response.data)
          }
          else {
            location.reload();
          }
        } else {
          console.log("OTA MODE ABORTED");
        }
        return
      }

      const response = await axios.get(`/${id}`);
      debug(response.data);
      const element = $(`#${id}`);
      element.text(response.data);
      if (response.data === "DEBUG DISABLED") {
        Logs = false;
      }
    } catch (error) {
      console.log("Main async function failed: ", error);
    }
  };


  const ids = []
  const elements = $("*")
  const array = Array.from(elements)
  array.forEach((val) => {
    if ($(val).attr("id") !== undefined)
      ids.push($(val).attr("id"))
  });

  async function checkStates() {

    const response = await axios({ url: "/getstates", method: "GET" });

    const data = response.data;
    debug("checkStates data:" + data);

    const arr = data.split(",")
    const newArr = []
    arr.forEach((val) => {
      newArr.push(val.replace(" ", ":").split(":"))
    })
    const Obj = Object.fromEntries(newArr)
    debug("entries:" + Obj)
    for (let a of Object.keys(Obj)) {
      debug(a + ":" + Obj[a])
      if (a.includes("slider")) {
        $(`#${a}`).val(Obj[a]);
        $(`#sliderValue`).text(Obj[a])
      } else {
        $(`#${a}`).text(Obj[a]);
      }
    }
  }

  function debug(msg) {
    if (Logs)
      console.log(msg)
  }

  async function getLocalIp() {
    const response = await axios({ url: "/getip", method: "GET" });
    console.log("ip response: ", response.data)
    localIp = response.data;
    $("#localip").text(localIp);
  }

  async function getMac(){
    const response = await axios(
      {
        url: "/getmac", method: "GET"
      })
      console.log("mac address: ", response.data)
      $("#macAddress"). text("MAC Addr.: " + response.data)
  }
  getMac();

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
        const id = e
          .target
          .id
        console
          .log("input event ", id)
        const xhr = new XMLHttpRequest();
        const url = id + "=" + $(slider).val();
        console.log("sliderValue = ", $(slider).val())
        const name = id
        $(SlidersValuesArr[index]).text(name + "'s speed: " + $(slider).val())

        const response = axios({ url: url, method: "GET" });

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

  /*
       $(`#${a}`).val(Obj[a]);
              $(`#sliderValue`).text(Obj[a])
  const response = await axios({
          url: "/getstates",
          method: "GET",
      });

      const data = response.data;
      debug("checkStates data:" + data);
      */

  let checkStatesInterval = null;
  let ipRetrieveInterval = null;

  function resumeIntervals() {
    scriptOn = true;
    $("#toggleIntervals").css({ "background-color": "blue", "color": "white" })
    console.log("resuming intervals");
    checkStatesInterval = setInterval(checkStates, 1000);
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