import express from 'express'
import path from 'path'
import {requestTime} from './middlewares.js'
import serverRoutes from './routes/servers.js'


const PORT = process.env.PORT ?? 3000
const __dirnname = path.resolve()
const app = express()

app.set('view engine', 'ejs')
app.set('views', path.resolve(__dirnname, 'ejs'))

app.use(express.static(path.resolve(__dirnname, 'static')))
app.use(requestTime)
app.use(serverRoutes)

app.get('/', (req, res) => {
    console.log('Main page opened')
    res.render('index', {title: 'MAIN PAGE', active: 'main'})             
})

app.get('/image', (req, res) => {
    console.log('Img page opened')
    res.render('image', {title: 'IMAGE PAGE', active: 'image'})
})

app.get('/prompt', (req, res) => {
    console.log('Img page opened')
    res.render('prompt', {title: 'PROMPT PAGE', active: 'prompt'})
})

app.get('/sheet', (req, res) => {
    console.log('Img page opened')
    res.render('sheet', {title: 'SHEET PAGE', active: 'sheet'})
})

app.listen(PORT, () => {
    console.log(`Server has started on port ${PORT}....`)
})