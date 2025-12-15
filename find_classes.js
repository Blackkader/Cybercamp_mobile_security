Java.perform(function () {
    Java.enumerateLoadedClasses({
        onMatch: function (className) {
            if (className.toLowerCase().includes("emul") || className.toLowerCase().includes("rjsniffer")) {
                console.log(className);
            }
        },
        onComplete: function () {
            console.log("Class enumeration complete");
        }
    });
});
