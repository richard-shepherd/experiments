(function() {

    window.addEventListener('DOMContentLoaded', function() {
        var isStreaming = false,
            video = document.getElementById('video'),
            canvas1 = document.getElementById('canvas1'),
            canvas2 = document.getElementById('canvas2'),
            grey = document.getElementById('grey');
        var info = document.getElementById('info');
        var canvasContext1 = canvas1.getContext('2d');
        var canvasContext2 = canvas2.getContext('2d');
        var w = 60,
            h = 42,
            greyscale = false;

        // Cross browser
        navigator.getUserMedia = (navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia);
        if (navigator.getUserMedia) {
            // Request access to video only
            navigator.getUserMedia(
                {
                    video:true,
                    audio:false
                },
                function(stream) {
                    // Cross browser checks
                    var url = window.URL || window.webkitURL;
                    video.src = url ? url.createObjectURL(stream) : stream;
                    // Set the video to play
                    video.play();
                },
                function(error) {
                    alert('Something went wrong. (error code ' + error.code + ')');
                    return;
                }
            );
        }
        else {
            alert('Sorry, the browser you are using doesn\'t support getUserMedia');
            return;
        }

        // Wait until the video stream can play
        video.addEventListener('canplay', function(e) {
            if (!isStreaming) {
                // videoWidth isn't always set correctly in all browsers
                if (video.videoWidth > 0) h = video.videoHeight / (video.videoWidth / w);
                canvas1.setAttribute('width', w);
                canvas1.setAttribute('height', h);

                canvas2.setAttribute('width', w*10);
                canvas2.setAttribute('height', h*10);

                // Reverse the canvas image
                canvasContext1.translate(w, 0);
                canvasContext1.scale(-1, 1);

                isStreaming = true;
            }
        }, false);

        // Wait for the video to start to play
        video.addEventListener('play', function() {
            // Every 33 milliseconds copy the video image to the canvas
            setInterval(function() {
                if (video.paused || video.ended) return;
                canvasContext1.fillRect(0, 0, w, h);
                canvasContext1.drawImage(video, 0, 0, w, h);

                if (greyscale) goingGrey();

                canvasContext2.drawImage(canvas1, 0, 0, w*10, h*10);
            }, 33);
        }, false);

        var goingGrey = function() {
            var imageData = canvasContext1.getImageData(0, 0, w, h);
            var data = imageData.data;
            info.innerText = data.length;
            for (var i = 0; i < data.length; i += 4) {
                var bright = 0.34 * data[i] + 0.5 * data[i + 1] + 0.16 * data[i + 2];
                data[i] = bright;
                data[i + 1] = bright;
                data[i + 2] = bright;
            }
            canvasContext1.putImageData(imageData, 0, 0);
        }

        // When the grey button is clicked, toggle the greyness indicator
        grey.addEventListener('click', function() {	greyscale = !greyscale; }, false);

    })
})();