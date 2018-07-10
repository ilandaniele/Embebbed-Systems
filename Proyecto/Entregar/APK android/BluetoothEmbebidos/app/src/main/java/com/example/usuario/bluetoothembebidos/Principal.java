package com.example.usuario.bluetoothembebidos;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.sql.Time;
import java.util.Calendar;
import java.util.TimeZone;
import java.util.UUID;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;


public class Principal extends Activity {
    Button botonAbecedario, botonBandas, botonEnviarString, botonIncRet, botonDecRet, botonIncEsp, botonDecEsp, botonACDC, botonHora;
    EditText StringAEnviar;
    Handler bluetoothIn;

    final int handlerState = 0;             //used to identify handler message
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private StringBuilder recDataString = new StringBuilder();
    private int flagHora=0;
    private ConnectedThread mConnectedThread;

    // SPP UUID service - direccion que funciona para la mayoria de los dispositivos
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // String para la dirección MAC
    private static String direccionMAC = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_principal);


        botonAbecedario = (Button) findViewById(R.id.boton_abecedario);
        botonBandas = (Button) findViewById(R.id.boton_bandas);
        botonEnviarString = (Button) findViewById(R.id.boton_enviarString);
        botonIncRet = (Button) findViewById(R.id.boton_IncRetardo);
        botonDecRet = (Button) findViewById(R.id.boton_DecRetardo);
        botonIncEsp = (Button) findViewById(R.id.boton_IncEspaciado);
        botonDecEsp= (Button) findViewById(R.id.boton_DecEspaciado);
        StringAEnviar = (EditText) findViewById(R.id.editText_CampoString);
        botonACDC = (Button) findViewById(R.id.boton_acdc);
        botonHora = (Button) findViewById(R.id.boton_hora);
        btAdapter = BluetoothAdapter.getDefaultAdapter();       // get Bluetooth adapter
        checkBTState();

        botonEnviarString.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                flagHora=0;
                String envio="";
                envio = StringAEnviar.getText().toString().toUpperCase();
                mConnectedThread.write("#"+envio);
                Toast.makeText(getBaseContext(), "Modo string normal", Toast.LENGTH_SHORT).show();
            }
        });
        // Set up onClick listeners for buttons to send 1 or 0 to turn on/off LED
        botonAbecedario.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                flagHora=0;
                mConnectedThread.write("2");    // Send "#2" via Bluetooth
                Toast.makeText(getBaseContext(), "Modo abecedario", Toast.LENGTH_SHORT).show();
            }
        });

        botonBandas.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                flagHora=0;
                mConnectedThread.write("3");
                Toast.makeText(getBaseContext(), "Modo Rolling Stones", Toast.LENGTH_SHORT).show();
            }
        });

        botonIncRet.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {

                mConnectedThread.write("4");
                Toast.makeText(getBaseContext(), "Incrementamos el retardo", Toast.LENGTH_SHORT).show();
            }
        });
        botonDecRet.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                mConnectedThread.write("5");
                Toast.makeText(getBaseContext(), "Decrementamos el retardo", Toast.LENGTH_SHORT).show();
            }
        });
        botonIncEsp.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                mConnectedThread.write("6");
                Toast.makeText(getBaseContext(), "Incrementamos el espaciado", Toast.LENGTH_SHORT).show();
            }
        });
        botonDecEsp.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                mConnectedThread.write("7");
                Toast.makeText(getBaseContext(), "Decrementamos el espaciado", Toast.LENGTH_SHORT).show();
            }
        });
        botonACDC.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                flagHora=0;
                mConnectedThread.write("8");
                Toast.makeText(getBaseContext(), "Modo ACDC", Toast.LENGTH_SHORT).show();
            }
        });
        botonHora.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                if(flagHora == 1){
                    Toast.makeText(getBaseContext(), "Modo ya ha sido seleccionado", Toast.LENGTH_SHORT).show();
                }else {
                    flagHora = 1;
                    Toast.makeText(getBaseContext(), "Modo hora", Toast.LENGTH_SHORT).show();
                    rutinaHora(60000);
                }

            }
        });
    }

    public void rutinaHora(int milisegundos) {
        if (flagHora == 1) {
            int hora = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
            int minuto = Calendar.getInstance().get(Calendar.MINUTE);
            String min = minuto+"";
            if(minuto<10) {
                min = "0" + minuto;
            }
            Toast.makeText(getBaseContext(), "hora actual: " + hora + ":" + min, Toast.LENGTH_SHORT).show();
            mConnectedThread.write("*" + hora + ":" + min);
            Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                public void run() {
                    // acciones que se ejecutan tras los milisegundos
                    if (flagHora == 1) {
                        rutinaHora(60000);
                    }
                }
            }, milisegundos);
        }
    }
    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {

        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
        //creates secure outgoing connecetion with BT device using UUID
    }

    @Override
    public void onResume() {
        super.onResume();

        //Get MAC address from DeviceListActivity via intent
        Intent intent = getIntent();

        //Get the MAC address from the DeviceListActivty via EXTRA
        direccionMAC = intent.getStringExtra("direccionDispositivo");

        //Log.i("hola", "adress : " + address);
        //obtengo el dispositivo remoto
        BluetoothDevice dispositivo = btAdapter.getRemoteDevice(direccionMAC);

        try {
            btSocket = createBluetoothSocket(dispositivo);
        } catch (IOException e) {
            Toast.makeText(getBaseContext(), "La creacción del Socket fallo", Toast.LENGTH_LONG).show();
        }
        // Establish the Bluetooth socket connection.
        try
        {
            btSocket.connect();
        } catch (IOException e) {
            try
            {
                btSocket.close();
            } catch (IOException e2)
            {
                //insert code to deal with this
            }
        }
        mConnectedThread = new ConnectedThread(btSocket);
        mConnectedThread.start();

        //I send a character when resuming.beginning transmission to check device is connected
        //If it is not an exception will be thrown in the write method and finish() will be called
        mConnectedThread.write("x");
    }

    @Override
    public void onPause()
    {
        super.onPause();
        try
        {
            //Don't leave Bluetooth sockets open when leaving activity
            btSocket.close();
        } catch (IOException e2) {
            //insert code to deal with this
        }
    }

    //Checks that the Android device Bluetooth is available and prompts to be turned on if off
    private void checkBTState() {

        if(btAdapter==null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta bluetooth", Toast.LENGTH_LONG).show();
        } else {
            if (btAdapter.isEnabled()) {
            } else {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }

    //create new class for connect thread
    private class ConnectedThread extends Thread {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        //creation of the connect thread
        public ConnectedThread(BluetoothSocket socket) {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                //Create I/O streams for connection
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }



        //write method
        public void write(String input) {
            byte[] msgBuffer = input.getBytes();           //converts entered String into bytes

            try {
                mmOutStream.write(msgBuffer);                //write bytes over BT connection via outstream
            } catch (IOException e) {
                //if you cannot write, close the application
                Toast.makeText(getBaseContext(), "La Conexión fallo", Toast.LENGTH_LONG).show();
                finish();

            }
        }
    }
}