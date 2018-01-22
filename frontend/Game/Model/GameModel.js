'use strict';

const playerType = 'p';
const foodCellType = 'f';
const boostCellType = 'b';

const createActionType = 'c';
const deleteActionType = 'd';
const updateActionType = 'u';

class Model {
    constructor() {
        this.status = 0;
        this.id = 0;
        this.playerCells = new Map();
        this.foodCells = new Map();
        this.boostCells = new Map();
    }

    //TODO fix id & status
    initData(data) {
        this.status = data.status;
        this.id = data.id;
    }

    updateModel(data) {
        for (let i in data.objectType) {
            let cell = {
                id: data.objectId[i], event: data.actionType[i], type: data.objectType[i],
                x: data.objectX[i], y: data.objectY[i], r: data.objectR[i]
            };
            switch (cell.event) {
                case createActionType:
                    this.setObject(cell);
                    break;
                case updateActionType:
                    this.setObject(cell);
                    break;
                case deleteActionType:
                    this.delObject(cell);
                    break;
            }
        }
    }

    setObject(cell) {
        switch (cell.type) {
            case playerType:
                this.playerCells.set(cell.id, {x: cell.x, y: cell.y, r: cell.r, color: getColor()});
                break;
            case foodCellType:
                this.foodCells.set(cell.id, {x: cell.x, y: cell.y, r: cell.r, color: getColor()});
                break;
            case boostCellType:
                this.boostCells.set(cell.id, {
                    x: cell.x, y: cell.y, r: cell.r,
                    color: function () {
                        return getColor();
                    }
                });
                break;
        }
    }

    delObject(cell) {
        switch (cell.type) {
            case playerType:
                this.playerCells.delete(cell.id);
                break;
            case foodCellType:
                this.foodCells.delete(cell.id);
                break;
            case boostCellType:
                this.boostCells.delete(cell.id);
                break;
        }
    }

}

class User {
    constructor() {
        this.id = -1;
        this.x = -1;
        this.y = -1;
        this.r = 1000000;
        this.angle = 0;
        this.speed = 0;
    }

    initData(data) {
        this.id = data.id;
    }

    updateData(model) {
        this.x = model.playerCells.get(this.id).x;
        this.y = model.playerCells.get(this.id).y;
        this.r = model.playerCells.get(this.id).r;
    }

    // TODO remove or modify
    localUpdate() {
        this.x = user.x + Math.round(Math.cos(user.angle) * user.speed);
        this.y = user.y + Math.round(Math.sin(user.angle) * user.speed);
    }

    userMoved(dx, dy) {
        let hypo = Math.sqrt(dx * dx + dy * dy);

        this.angle = Math.round(Math.sign(dy) * Math.acos(dx / hypo) * 180 / Math.PI);

        let temp = Math.min(dx, dy) / LPtoDP(this.r);
        this.speed = DPtoLP(Math.abs(temp) >= 1 ? 1 : Math.abs(temp));
    }

    sendData() {
        return {
            packetType: "INPT",
            angle: parseInt(this.angle),
            speed: parseInt(this.speed)
        };
    }
}

function getColor() {
    return colors[Math.floor(Math.random() * colors.length)];
}


let colors = [
    "#E0E0E0",
    "#FF99CC",
    "#FF99FF",
    "#CC99FF",
    "#9999FF",
    "#99CCFF",
    "#99FFFF",
    "#99FFCC",
    "#99FF99",
    "#CCFF99",
    "#FFFF99",
    "#FFCC99",
    "#FF9999",
    "#808080",
    "#FF007F",
    "#FF00FF",
    "#7F00FF",
    "#0000FF",
    "#0080FF",
    "#00FFFF",
    "#00FF80",
    "#00FF00",
    "#80FF00",
    "#FF8000",
    "#FF0000",
    "#202002",
    "#660033",
    "#660066",
    "#330066",
    "#000066",
    "#003366",
    "#006666",
    "#006633",
    "#006600",
    "#336600",
    "#666600",
    "#663300",
    "#660000"
];