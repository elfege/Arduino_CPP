const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Water Level Monitor</title>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.7.2/font/bootstrap-icons.css">
    <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
    <style>
        body {
            display: flex;
            height: 100vh;
            justify-content: center;
            align-items: center;
            background-color: #e6f7ff;
        }

        #waterDrop {
            width: 100px;
            height: 150px;
            background-color: #0099FF;
            border-radius: 50% 50% 50% 50%/60% 60% 40% 40%;
            position: relative;
        }

        #state {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            color: white;
            font-weight: bold;
        }
    </style>
</head>

<body>
    <div class="container">
        <div class="row justify-content-center mb-4">
            <div class="col-sm-4 text-center">
                <div id="waterDrop" class="mb-3">
                    <p id="state">WATER LEVEL</p>
                </div>
            </div>
            <div class="col-sm-2 text-center">
                <button class="btn btn-outline-primary" id="reset">RESET</button>
            </div>
            <div class="col-sm-4 text-center">
                <div id="temperatureDisplay">
                    <h3><span id="tempValue">--</span>°F</h3>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-12">
                <h4 id="serial" class="text-center mb-3">Serial Output</h4>
                <div class="d-flex justify-content-center">
                    <iframe id="terminal" src="http://192.168.10.224/term.html"
                        style="width:100%; max-width:800px; height:600px;"></iframe>
                </div>
            </div>
        </div>
    </div>


</body>

<script>
    const MainFunc = async (id) => {
        try {
            if (id === "reset") {
                console.debug("Initiating reset...");
                try {
                    const response = await axios.get("http://192.168.10.224/reset", {
                        // Add timeout
                        timeout: 5000,
                        // Handle CORS for development
                        withCredentials: false,
                        headers: {
                            'Accept': 'application/json',
                            'Content-Type': 'application/json'
                        }
                    });
                    console.debug("Reset response:", response.data);
                    alert("Reset successful");
                } catch (error) {
                    console.error("Full error object:", error);
                    let errorMessage = "Reset failed: ";

                    if (error.code === 'ECONNABORTED') {
                        errorMessage += "Request timed out";
                    } else if (error.code === 'ERR_NETWORK') {
                        errorMessage += "Network error - device might be offline";
                    } else if (error.response) {
                        // Server responded with error
                        errorMessage += `Server returned ${error.response.status}: ${error.response.statusText}`;
                    } else if (error.request) {
                        // Request made but no response
                        errorMessage += "No response from device";
                    } else {
                        // Other errors
                        errorMessage += error.message || "Unknown error occurred";
                    }

                    console.error("Detailed error:", errorMessage);
                    alert(errorMessage);
                }
                return;
            }

            const response = await axios.get(`/${id}`);
            console.debug(`${id} response:`, response.message);
        } catch (error) {
            console.error("Request failed:", error);
            let errorMessage;

            if (error.response) {
                errorMessage = `Error: ${error.response.status} - ${error.response.statusText}`;
            } else if (error.request) {
                errorMessage = "Error: Request made but no response received";
            } else if (error.code === 'ECONNABORTED') {
                errorMessage = "Error: Request timed out";
            } else {
                errorMessage = `Error: ${error.message}`;
            }

            console.error("Detailed error info:", errorMessage);
            alert(errorMessage);
        }
    };

    $(() => {
        let intervals = {
            waterDrop: null,
            temperature: null
        };

        $("#reset").on("click", () => {
            //{# axios.get("http://192.168.10.224/reset"); #}
            MainFunc("reset")
        });

        const updateWaterLevel = async () => {
            try {
                const response = await axios.get("/dropstate");
                const data = response.data;

                if (data === "LEVEL IS UP") {
                    $("#state").text("WATER LEVEL IS: UP");
                    $("#waterDrop").css("background-color", "blue");
                } else if (data === "LEVEL IS LOW") {
                    $("#state").text("WATER LEVEL IS: LOW");
                    $("#waterDrop").css("background-color", "darkgrey");
                }
            } catch (error) {
                console.error("Water level check failed:", error.message);
                $("#state").text("API ERROR");
                $("#waterDrop").css("background-color", "red");
            }
        };

        const updateTemperature = async () => {
            try {
                const response = await axios.get("/temperature");
                $("#tempValue").text(response.data);
            } catch (error) {
                console.error("Temperature check failed:", error.message);
                $("#tempValue").text("--");
            }
        };

        // Initial updates
        updateWaterLevel();
        updateTemperature();

        // Set intervals
        intervals.waterDrop = setInterval(updateWaterLevel, 2000);
        intervals.temperature = setInterval(updateTemperature, 5000);

        // Session timeout handler
        const SESSION_TIMEOUT = 600000; // 10 minutes
        const clearAllIntervals = () => {
            Object.values(intervals).forEach(interval => {
                if (interval) clearInterval(interval);
            });
            intervals = { waterDrop: null, temperature: null };

            alert("Session timeout reached. Please refresh the page for live updates.");
        };

        // Set timeout to clear intervals
        setTimeout(clearAllIntervals, SESSION_TIMEOUT);

        // Clean up on page unload
        $(window).on('unload', clearAllIntervals);
    });
</script>


</html>

)=====";