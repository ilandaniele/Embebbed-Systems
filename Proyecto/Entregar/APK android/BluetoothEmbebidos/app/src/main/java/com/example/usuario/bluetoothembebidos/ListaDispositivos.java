package com.example.usuario.bluetoothembebidos;

import android.app.Activity;
import java.util.Set;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;

import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;
/**
 * Created by Usuario on 27/11/2017.
 */

public class ListaDispositivos extends Activity {

        // Debugging for LOGCAT
        private String TAG = "ActividadListaDispositivos";


        //etiqueta que aparece cuando selecciono un objeto de la lista
        TextView etiquetaConectando;

        // Member fields
        private BluetoothAdapter mBtAdapter;
        private ArrayAdapter AdaptadorDispositivosApareados;

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.lista_dispositivos);
        }

        @Override
        public void onResume()
        {
            super.onResume();
            //***************
            checkBTState();

            //esta se mostrará en la carga, cuando seleccione un dispositivo para enlazar al bluetooth
            etiquetaConectando = (TextView) findViewById(R.id.conectando);
            etiquetaConectando.setTextSize(40);
            etiquetaConectando.setText(" ");

            //Creo el adaptador para los dispositivos a mostrar
            AdaptadorDispositivosApareados = new ArrayAdapter(this, R.layout.item_dispositivo);

            //Creo la lista que mostrara todos los dispositivos apareados
            ListView ListViewDispositivosApareados = (ListView) findViewById(R.id.listaDispositivosApareados);
            ListViewDispositivosApareados.setAdapter(AdaptadorDispositivosApareados);
            ListViewDispositivosApareados.setOnItemClickListener(OyenteDispositivoSeleccionado);

            // Get the local Bluetooth adapter
            mBtAdapter = BluetoothAdapter.getDefaultAdapter();

            // Get a set of currently paired devices and append to 'pairedDevices'
            Set pairedDevices = mBtAdapter.getBondedDevices();

            // Agrego los dispositivos apareados anteriores a la lista
            if (pairedDevices.size() > 0) {
                //hago que aparezca el titulo de la lista
                findViewById(R.id.tituloListaDispositivosEmparejados).setVisibility(View.VISIBLE);
                for (BluetoothDevice device : mBtAdapter.getBondedDevices()) {
                    AdaptadorDispositivosApareados.add(device.getName() + "\n" + device.getAddress());
                }
            } else {
                String noDevices = "Ningun dispositivo pudo ser emparejado";
                AdaptadorDispositivosApareados.add(noDevices);
            }
        }

        // armo el listener para el dispositivo seleccionado
        private OnItemClickListener OyenteDispositivoSeleccionado = new OnItemClickListener() {
            public void onItemClick(AdapterView av, View v, int arg2, long arg3) {

                //v es el item de la lista
                etiquetaConectando.setText("Conectando...");
                // Get the device MAC address, which is the last 17 chars in the View
                String info = ((TextView) v).getText().toString();
                String address = info.substring(info.length() - 17);

                // Make an intent to start next activity while taking an extra which is the MAC address.
                Intent intento = new Intent(ListaDispositivos.this, Principal.class);
                intento.putExtra("direccionDispositivo", address); //le paso la direccion a la nueva actividad
                startActivity(intento);
            }
        };

        private void checkBTState() {
            // Check device has Bluetooth and that it is turned on
            mBtAdapter=BluetoothAdapter.getDefaultAdapter(); // CHECK THIS OUT THAT IT WORKS!!!
            if(mBtAdapter==null) {
                Toast.makeText(getBaseContext(), "El dispositivo no soporta Bluetooth", Toast.LENGTH_SHORT).show();
            } else {
                if (mBtAdapter.isEnabled()) {
                    Log.d(TAG, "...Bluetooth Activado...");
                } else {
                    //Prompt user to turn on Bluetooth
                    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBtIntent, 1);

                }
            }
        }
}

