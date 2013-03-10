# hiptext - Render Images as Text

![Last Supper in Macterm](http://i.imgur.com/7TfrQsL.png)

![Happy Cat in Macterm](http://i.imgur.com/cr7sSHh.png)

![Obama in Macterm](http://i.imgur.com/UnfnpMr.png)

Hiptext is a fun little research project of Justine Tunney to find new and
interesting ways to render images as text, with a lot of help from Serene Han.

## Dependencies

You need clang or gcc >=4.7. You also need to install:

    sudo apt-get install build-essential libpng12-dev libjpeg-dev \
        libfreetype6-dev libgif-dev ragel libavformat-dev libavcodec-dev \
        libswscale-dev
    sudo ./install-glog.sh
    sudo ./install-gflags.sh
    sudo ldconfig

## Screenshots

- --nocolor: http://imgur.com/a/IQQcj
- --256color: http://imgur.com/a/YY3kR
- --256color_hack1: http://imgur.com/a/X30vd

## Demo Invocation

    make
    ./hiptext
    ./hiptext --nocolor
    ./hiptext --nocolor --chars=" .oO0"
    ./hiptext --hinting
