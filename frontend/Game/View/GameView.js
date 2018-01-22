class View {
    constructor(canvasName) {
        this.canvas = document.getElementById(canvasName);
        this.canvas.height = window.innerHeight;
        this.canvas.width = window.innerWidth;
        this.ctx = this.canvas.getContext("2d");
    }

    render(model, user) {
        this.drawGame(user, model.playerCells, model.foodCells, model.boostCells);
    }

    drawGame(user, players, foodCells, boostCells) {
        this.clearField(0, 0, this.canvas.width, this.canvas.height);
        // Draw food cells
        this.drawCellsMap(foodCells);
        // Draw boost cells
        this.drawCellsMap(boostCells);
        // Draw player cells
        this.drawCellsMap(players);
    }

    drawCellsMap(map) {
        for (let value of  map.values())
            this.drawCell(LPtoDP(value.x), LPtoDP(value.y), LPtoDP(value.r),
                value.color, LPtoDP(user.x), LPtoDP(user.y));
    }

    drawLost(handle) {
        // ...
    }

    drawCell(x, y, r, color, userX, userY) {
        if (Math.abs(x - userX) <= this.canvas.width / 2 + r && Math.abs(y - userY) <= this.canvas.height / 2 + r)
            drawCircle(this.ctx, this.canvas.width / 2 + x - userX, this.canvas.height / 2 + y - userY, r, color);
    }

    clearField(x, y, width, height) {
        this.ctx.beginPath();
        this.ctx.clearRect(0, 0, width, height);
        this.ctx.closePath();
    }


}

function drawCircle(ctx, x, y, r, color) {
    ctx.beginPath();
    ctx.arc(x, y, r, 0, Math.PI * 2);
    if (typeof(color)!=="function")
        ctx.fillStyle = color;
    else
        ctx.fillStyle = color();
    ctx.fill();
    ctx.closePath();
}


function LPtoDP(LPoint) {
    let DPoint;
    DPoint = Math.round(LPoint / 1000);
    return DPoint
}

function DPtoLP(DPoint) {
    let LPoint;
    LPoint = Math.round(DPoint * 1000);
    return LPoint
}