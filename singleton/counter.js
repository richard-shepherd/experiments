
function Counter() {
    this.count = 0;
}

Counter.prototype.increment = function() {
    this.count++;
};

Counter._instance = null;

Counter.getInstance = function() {
    if(Counter._instance == null) {
        Counter._instance = new Counter();
    }
    return Counter._instance;
};


