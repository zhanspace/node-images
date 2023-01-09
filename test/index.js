var imagesFn = require(__dirname + '/../');
var images = function (file) { return imagesFn(__dirname + '/' + file); };
images("input.png")
    .resize( 200 )
    .save("./test/output_new.png");

images("input.png")
    .rotate( 45 )
    .save("./test/output_rotate.png");

images("input.png")
    .size( 200 )
    .save("./test/output_old.png");


images("input.jpg")
    .resize( 200 )
    .save("./test/output_new.jpg");


images("input.jpg")
    .size( 200 )
    .save("./test/output_old.jpg");

images("input.gif")
    .resize( 200 )
    .save("./test/output_new_gif.jpg");


images("input.gif")
    .size( 200 )
    .save("./test/output_old_gif.jpg");
