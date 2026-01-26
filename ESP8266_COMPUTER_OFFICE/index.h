const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Computer Sleep Control</title>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
</head>
<body>
    <div class="container">
        <div class="text-center mt-5">
            <h1 class="mb-4">Computer Sleep Control</h1>
        </div>
        <div class="row mt-3">
            <div class="col-md-4">
                <button id="powerstatus" class="btn btn-outline-primary btn-block" onclick="MainFunc('/togglepower', 'powerstatus')">Switch</button>
            </div>
            <div class="col-md-4">
                <button id="override" class="btn btn-outline-primary btn-block" onclick="MainFunc('/override', 'override')">Override</button>
                <button id="nosleep" class="btn btn-outline-primary btn-block" onclick="MainFunc('/nosleep', 'nosleep')">Don't Sleep</button>
                <button id="shutdownbool" class="btn btn-outline-primary btn-block" onclick="MainFunc('/shutdownbooltoggle', 'shutdownbool')">Allow Shutdown</button>
            </div>
            <div class="col-md-4">
                <button id="hard_reboot" class="btn btn-outline-danger btn-block" onclick="MainFunc('/hard_reboot', 'hard_reboot')">Hard Reboot</button>
                <button id="shutDown" class="btn btn-outline-danger btn-block" onclick="MainFunc('/shutDown', 'shutDown')">Shutdown</button>
                <button id="reset" class="btn btn-outline-secondary btn-block" onclick="MainFunc('/reset', 'reset')">Reset</button>
            </div>
        </div>
        <div class="row mt-4">
            <div class="col-md-6">
                <textarea id="timeInfo" class="form-control mb-2" rows="1" readonly></textarea>
                <textarea id="macinfo" class="form-control" rows="1" readonly></textarea>
            </div>
            <div class="col-md-6">
                <iframe width="100%" height="300" src="http://192.168.10.190/term.html" title="Serial Terminal Output" style="border:1px solid #ccc;"></iframe>
            </div>
        </div>
    </div>
    <script>
        const MainFunc = async (name, id) => {
            try {
                const response = await axios.get(name);
                $(`#${id}`).text(response.data);
                console.log(response.data);
            } catch (error) {
                console.error(error);
            }
        };

        const getMac = async () => {
            try {
                const response = await axios.get("/getMac");
                $("#macinfo").text(response.data);
                console.log(response.data);
            } catch (error) {
                console.error(error);
            }
        };

        const getTimeInfo = async () => {
            try {
                const response = await axios.get("/getTimeInfo");
                $("#timeInfo").text(response.data);
                console.log(response.data);
            } catch (error) {
                console.error(error);
            }
        };

        const getpowerstate = async () => {
            try {
                const response = await axios.get("/getpowerstate");
                $("#powerstatus").text(response.data);
                console.log(response.data);
            } catch (error) {
                console.error(error);
            }
        };

        const getDebug = async () => {
            try {
                const response = await axios.get("/debug");
                $("#debug").html(response.data);
                $("#debug").scrollTop($("#debug")[0].scrollHeight);
            } catch (error) {
                console.error(error);
            }
        };

        // Initialize your functions when the document is ready
        $(document).ready(() => {
            getMac();
            getTimeInfo();
            MainFunc("/overrideCheck", "override");
            MainFunc("/shutdownbool", "shutdownbool");
            MainFunc("/nosleepcheck", "nosleep");
            const interval1 = setInterval(() => getpowerstate(), 2000);
            const interval2 = setInterval(() => getDebug(), 1000);
            const interval3 = setInterval(() => getTimeInfo(), 5000);
            const interval4 = setInterval(() => MainFunc("/overrideCheck", "override"), 5000);
            const interval5 = setInterval(() => MainFunc("/shutdownbool", "shutdownbool"), 5000);
            setTimeout(() => clearAll(), 1800 * 1000); // clear all intervals after 30 minutes
        });

        const clearAll = () => {
            clearInterval(interval1);
            clearInterval(interval2);
            clearInterval(interval3);
            clearInterval(interval4);
            clearInterval(interval5);
            alert("All scheduled tasks canceled; refresh this page to resume");
        };
    </script>
</body>
</html>
)=====";
