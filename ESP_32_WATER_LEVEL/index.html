const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Water Level Monitor</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" 
          integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" 
          crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.7.2/font/bootstrap-icons.css">
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
    <div id="waterDrop">
        <p id="state">WATER LEVEL</p>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js" 
            integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM" 
            crossorigin="anonymous"></script>
</body>
<script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
<script>
    $(document).ready(function(){
        setInterval(function(){
            $.get("/dropstate", function(data){
                if(data === "LEVEL IS UP") {
                    $("#state").text("WATER LEVEL IS: UP");
                    $("#waterDrop").css("background-color", "blue");
                } else if(data === "LEVEL IS LOW") {
                    $("#state").text("WATER LEVEL IS: LOW");
                    $("#waterDrop").css("background-color", "darkgrey");
                }
            });
        }, 500); // Refresh every 0.5 seconds
    });
</script>


</html>

)=====";