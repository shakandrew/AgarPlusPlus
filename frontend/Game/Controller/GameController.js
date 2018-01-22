const canvasName = "gameCanvas";
document.addEventListener("mousemove", mouseMoveHandler, false);
let waitForConnectionId = null;
let initDataId = null;
let getDataId = null;
let runId = null;
let sendUserDataId = null;

let view = null;
let model = null;
let user = null;
let data = null;

function start() {
    view = new View(canvasName);
    model = new Model();
    user = new User();
    loading();
}

function initData() {
    if (serverData !== null) {
        data = JSON.parse(serverData);
        model.initData(data);
        user.initData(data);
        //console.log(serverData);
        clearInterval(initDataId);
        runId = setInterval(run, 20);
        getDataId = setInterval(getData, 50);
        sendUserDataId = setInterval(sendUserData, 50);
    }
}

function getData() {
    if (data !== serverData) {
        data = serverData;
        let parsedData = JSON.stringify(data);
        // switch case
        if (parsedData.state === "") {
            clearInterval(getDataId);
            clearInterval(runId);
            clearInterval(sendUserDataId);
            view.drawLost();
            document.getElementById("startBtn").style.display = "block";
        }
        else {
            model.updateModel(parsedData);
            user.updateData(model);
        }
    }
}


function run() {
    view.render(model, user);
}

//TODO User.sendData function
function sendUserData() {
    sendMessage(JSON.stringify(user.sendData()));
}

function loading() {
    // html design
    document.getElementById("startBtn").style.display = "none";
    document.getElementById("gameLoader").style.display = "block";

    establish_connection();
}

function establish_connection() {
    connect();
    waitForConnectionId = setInterval(waitForConnection, 1000);
}

function waitForConnection() {
    if (connection_established) {
        // html design
        document.getElementById("gameLoader").style.display = "none";
        document.getElementById("gameCanvas").style.display = "block";

        clearInterval(waitForConnectionId);
        // send message to connect the server
        sendMessage(JSON.stringify({packetType: "JOIN", playerName: ""}));
        initDataId = setInterval(initData, 100);
    }
}

function mouseMoveHandler(event) {
    if (user !== null) {
        let dx = event.clientX - view.canvas.width / 2;
        let dy = event.clientY - view.canvas.height / 2;
        user.userMoved(dx, dy);
    }
}
