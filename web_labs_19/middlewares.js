// req и res являются обынычми объектами, которым можно добавлять свойства. Middleware позволяет это делать

export function requestTime(req, res, next) {
    req.requestTime = Date.now()
    next()
}

export function logger(req, res, next){
    console.log(colors.bgGreen.black(`Request time: ${rew.requestTime}`))
}