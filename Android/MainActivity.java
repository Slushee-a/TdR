/*
 * I'd rather add too many comments than too few.
 * Code is easier to write than it is to read, so let's not risk it.
 * Made by Slushee (Pol Fernàndez)
 * Alpha 2.0.2 (19/05/2021)
 */


//Name the package and import classes/libraries
package com.firstapp.app;

import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.util.Objects;

//Main Activity Code
public class MainActivity extends AppCompatActivity
{

    EditText IPsegment1text;
    EditText IPsegment2text;
    EditText IPsegment3text;
    EditText IPsegment4text;

    String GetIPsegment1;
    String GetIPsegment2;
    String GetIPsegment3;
    String GetIPsegment4;

    String url;

    Button ConnectButton;

    private SeekBar seekBar1;                                                             // Define the seekbar 1 (left motor)
    private SeekBar seekBar2;                                                             // Define the seekbar 2 (right motor)
    Switch SafeMode;                                                                      // Define the Safe Mode switch
    Boolean SafeModeState = false;                                                        // Create a boolean to store the the state of the safe mode.


    @Override
    protected void onCreate(Bundle savedInstanceState)                                    // When the app is running on the foreground
    {
        requestWindowFeature(Window.FEATURE_NO_TITLE);                                    // Remove the app title
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // Set the app as fullscreen
        Objects.requireNonNull(getSupportActionBar()).hide();                             // Hide the actionbar (The top bar with the hour and notifications)

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        IPsegment1text = (EditText)findViewById(R.id.IPsegment1);
        IPsegment2text = (EditText)findViewById(R.id.IPsegment2);
        IPsegment3text = (EditText)findViewById(R.id.IPsegment3);
        IPsegment4text = (EditText)findViewById(R.id.IPsegment4);

        ConnectButton = (Button)findViewById(R.id.ConnectB);

        ConnectButton.setOnClickListener(new View.OnClickListener()
        {

            @Override public void onClick(View v)
             {
                GetIPsegment1 = IPsegment1text.getText().toString();
                GetIPsegment2 = IPsegment2text.getText().toString();
                GetIPsegment3 = IPsegment3text.getText().toString();
                GetIPsegment4 = IPsegment4text.getText().toString();
                url = "http://" + GetIPsegment1 + "." + GetIPsegment2 + "." + GetIPsegment3 + "." + GetIPsegment4 + "/";

                RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                   // Create a new request queue
                StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"STOP", new Response.Listener<String>()       // Make a request for the motor to stop
                {
                    @Override
                    public void onResponse(String response)                                                                               // If there is a response from the server,
                    {
                        Toast.makeText(MainActivity.this,"Connected",Toast.LENGTH_SHORT).show();                             // Notify the user that the robot has connected successfully.
                    }
                },
                   new Response.ErrorListener()                                                                                           // Check for errors
                   {
                      @Override
                      public void onErrorResponse(VolleyError error)                                                                      // If there is an error
                      {
                               Toast.makeText(MainActivity.this,"Robot not found",Toast.LENGTH_SHORT).show();                // Warn the user that the motor couldn't be stopped because the robot is not connected.
                      }
                   }
                );
                 queue.add(stringRequest);
             }
        });


        seekBar1 = (SeekBar) findViewById(R.id.seekBar1);                                 // Define the seekbar 1's ID
        seekBar2 = (SeekBar) findViewById(R.id.seekBar2);                                 // Define the seekbar 2's ID
        SafeMode = findViewById(R.id.SafeMode);                                           // Define the Safe Mode switch's ID



        SafeMode.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()  // Check if the Safe Mode switch has changed state
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)    // If the state has changed
            {
                if (SafeMode.isChecked())                                                 // If the Switch is on,
                {
                    SafeModeState = true;                                                 // Set the safe mode state boolean as true.
                }
                else                                                                      // If it isn't on,
                {
                    SafeModeState = false;                                                // Set the safe mode state boolean as false.
                }
            }
        });


        seekBar1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()                                                           // Check if the seekbar 1 has changed state
        {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)                                                  // If it has changed state
            {

                RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                     // Create a new request queue and
                StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"1/"+progress, new Response.Listener<String>()  // make a request with the progress value of the seekbar. Listen for a response.
                {
                    @Override
                    public void onResponse(String response)                                                                                  // If there is a response from the server
                    {
                                                                                                                                             // Do nothing
                    }

                },

                    new Response.ErrorListener()                                                                                             // Check for errors
                    {
                        @Override
                        public void onErrorResponse(VolleyError error)                                                                       // If there has been an error
                        {
                            Toast.makeText(MainActivity.this,"Robot is not connected.",Toast.LENGTH_LONG).show();               // Warn the user that the robot is not connected
                        }
                    }
                );
                queue.add(stringRequest);                                                                                                     // Add the request to the queue
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar)                                                                                 // If the seekbar is being touched
            {
                                                                                                                                              // Do nothing
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar)                                                                                  // If the seekbar is no longer being touched and
            {
                if(SafeModeState)                                                                                                             // Safe mode is enabled,
                {
                    RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                   // Create a new request queue
                    StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"1/0", new Response.Listener<String>()        // Make a request for the motor to stop
                    {
                        @Override
                        public void onResponse(String response)                                                                                // If there is a response from the server,
                        {
                                                                                                                                               // Do nothing
                        }
                    },
                        new Response.ErrorListener()                                                                                           // Check for errors
                        {
                            @Override
                            public void onErrorResponse(VolleyError error)                                                                     // If there is an error
                            {
                               Toast.makeText(MainActivity.this,"Robot is not connected. Motors couldn't be stopped",Toast.LENGTH_LONG).show();  // Warn the user that the motor couldn't be stopped because the robot is not connected.
                            }
                        }
                    );
                   queue.add(stringRequest);                                                                                                    // Add the request to the queue
                }
            }
        });

        seekBar2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()                                                               // Check if the seekbar 1 has changed state
        {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)                                                      // If it has changed state
            {

                RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                         // Create a new request queue and
                StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"2/"+progress, new Response.Listener<String>()      // make a request with the progress value of the seekbar. Listen for a response.
                {
                    @Override
                    public void onResponse(String response)                                                                                      // If there is a response from the server
                    {
                                                                                                                                                 // Do nothing
                    }

                },

                     new Response.ErrorListener()                                                                                                // Check for errors
                     {
                         @Override
                         public void onErrorResponse(VolleyError error)                                                                          // If there has been an error
                         {
                            Toast.makeText(MainActivity.this,"Robot is not connected.",Toast.LENGTH_LONG).show();                   // Warn the user that the robot is not connected
                         }
                     }
                );
                queue.add(stringRequest);                                                                                                         // Add the request to the queue
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar)                                                                                     // If the seekbar is being touched
            {
                                                                                                                                                  // Do nothing
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar)                                                                                      // If the seekbar is no longer being touched and
            {
                if(SafeModeState)                                                                                                                 // Safe mode is enabled,
                {
                    RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                       // Create a new request queue
                    StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"2/0", new Response.Listener<String>()            // Make a request for the motor to stop
                    {
                        @Override
                        public void onResponse(String response)                                                                                   // If there is a response from the server,
                        {
                                                                                                                                                  // Do nothing
                        }
                    },
                            new Response.ErrorListener()                                                                                          // Check for errors
                            {
                                @Override
                                public void onErrorResponse(VolleyError error)                                                                    // If there has been an error
                                {
                                    Toast.makeText(MainActivity.this,"Robot is not connected, motors couldn't be stopped.",Toast.LENGTH_LONG).show();  // Warn the user that the motor couldn't be stopped because the robot is not connected.
                                }
                            }
                    );
                    queue.add(stringRequest);                                                                                                     // Add the request to the queue
                }
            }
        });
    }


    @Override
    protected void onDestroy()                                                                                                          // When the app is closed
    {
        super.onDestroy();

        RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                         // Create a new request queue and
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"STOP", new Response.Listener<String>()              // make a request to turn of the motors. Listen for a response.
        {
            @Override
            public void onResponse(String response)                                                                                      // If there is a response from the server
            {
                                                                                                                                         // Do nothing
            }

        },
             new Response.ErrorListener()                                                                                             // Check for errors
             {
                 @Override
                 public void onErrorResponse(VolleyError error)                                                                          // If there has been an error
                 {
                     Toast.makeText(MainActivity.this,"Robot is not connected, robot couldn't be stopped.",Toast.LENGTH_LONG).show();  // Warn the user that the robot couldn't be stopped because the robot is not connected.
                 }
             }
        );
        queue.add(stringRequest);                                                                                                        // Add the request to the queue
         // Add the request to the queue
    }

}

