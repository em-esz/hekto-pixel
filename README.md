OTA firmware update form available under

http://hektopixel/update

With 

POST /animation/play/{name} you can start an animation

Available animations
- plasma
- text - displays a scrolling text
- noise - sets every pixel on the board to a random color
- artnet - acts as an ArtNet client


# ArtNet settings

LED type GRB
Universe 1: 20 x 8, LED starting top - left, snakewise ( only 0 - 480 channels are used )
Universe 2: 20 x 7, LED starting top - left, snakewise ( only 0 - 420 channels are used )

# web GUI - go to /web

## Project setup
```
npm install
```

### Compiles and hot-reloads for development
```
npm run serve
```

### Compiles and minifies for production
```
npm run build
```

### Lints and fixes files
```
npm run lint
```

### Customize configuration
See [Configuration Reference](https://cli.vuejs.org/config/).
