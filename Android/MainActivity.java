/*
 * I'd rather add too many comments than too few.
 * Code is easier to write than it is to read, so let's not risk it.
 * Made by Slushee (Pol Fernàndez)
 * Alpha 2.1.4 (13/06/2021)
 */


//Name the package and import classes/libraries
package com.firstapp.app;

import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
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

    EditText IPsegment1text;                                                              // Start the first edittext segment
    EditText IPsegment2text;                                                              // Start the second edittext segment
    EditText IPsegment3text;                                                              // Start the third edittext segment
    EditText IPsegment4text;                                                              // Start the bot's fourth edittext segment
    EditText IPsegment5text;                                                              // Start the camera's fourth edittext segment

    String GetIPsegment1;                                                                 // Start the variable that will hold the first edittext segment value
    String GetIPsegment2;                                                                 // Start the variable that will hold the second edittext segment value
    String GetIPsegment3;                                                                 // Start the variable that will hold the third edittext segment value
    String GetIPsegment4;                                                                 // Start the variable that will hold the bot's fourth edittext segment value
    String GetIPsegment5;                                                                 // Start the variable that will hold the camera's fourth edittext segment value

    String url;                                                                           // Start the variable that will hold the url for the bot
    String camurl;                                                                        // Start the variable that will hold the url for the camera

    Button ConnectButton;                                                                 // Define the connect button
    ImageButton CamButton;                                                                // Define the take a picture button

    private SeekBar seekBar1;                                                             // Define the seekbar 1 (left motor)
    private SeekBar seekBar2;                                                             // Define the seekbar 2 (right motor)

    Switch SafeMode;                                                                      // Define the Safe Mode switch
    Switch FlashSwitch;                                                                   // Define the flash's switch

    Boolean SafeModeState = false;                                                        // Create a boolean to store the the state of the safe mode.
    private WebView Camera;

    @Override
    protected void onCreate(Bundle savedInstanceState)                                    // When the app is running on the foreground
    {
        requestWindowFeature(Window.FEATURE_NO_TITLE);                                    // Remove the app title
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); // Set the app as fullscreen
        Objects.requireNonNull(getSupportActionBar()).hide();                             // Hide the actionbar (The top bar with the hour and notifications)

        super.onCreate(savedInstanceState);                                               // Create instance
        setContentView(R.layout.activity_main);                                           // Set the screen to the activity

        IPsegment1text = (EditText)findViewById(R.id.IPsegment1);                         // Define the id of the first edittext segment (regularly 192)
        IPsegment2text = (EditText)findViewById(R.id.IPsegment2);                         // Define the id of the second edittext segment (regularly 168)
        IPsegment3text = (EditText)findViewById(R.id.IPsegment3);                         // Define the id of the third edittext segment (regularly 1)
        IPsegment4text = (EditText)findViewById(R.id.IPsegment4);                         // Define the id of the bot's fourth edittext segment
        IPsegment5text = (EditText)findViewById(R.id.IPsegment5);                         // Define the id of the camera's fourth edittext segment

        ConnectButton = (Button)findViewById(R.id.ConnectB);                              // Define the id of the connect button
        CamButton = (ImageButton)findViewById(R.id.CamButton);                            // Define the id of the take a picture button

        seekBar1 = (SeekBar) findViewById(R.id.seekBar1);                                 // Define the seekbar 1's ID
        seekBar2 = (SeekBar) findViewById(R.id.seekBar2);                                 // Define the seekbar 2's ID

        SafeMode = findViewById(R.id.SafeMode);                                           // Define the Safe Mode switch's ID

        FlashSwitch = findViewById(R.id.FlashSwitch);                                     // Define the flash switch's ID

        Camera = (WebView)findViewById(R.id.camera);                                      // Define the id of the camera web view
        Camera.setWebViewClient(new WebViewClient());                                     // Start a new client in the web view

        ConnectButton.setOnClickListener(new View.OnClickListener()                       // Listen for clicks in the connect button
        {

            @Override public void onClick(View v)                                         // When a click is registered
             {
                GetIPsegment1 = IPsegment1text.getText().toString();                      // Get the value of the first segment
                GetIPsegment2 = IPsegment2text.getText().toString();                      // Get the value of the second segment
                GetIPsegment3 = IPsegment3text.getText().toString();                      // Get the value of the third segment
                GetIPsegment4 = IPsegment4text.getText().toString();                      // Get the value of the bot's fourth segment
                GetIPsegment5 = IPsegment5text.getText().toString();                      // Get the value of the camera's fourth segment

                url = "http://" + GetIPsegment1 + "." + GetIPsegment2 + "." + GetIPsegment3 + "." + GetIPsegment4 + "/";  // Set the bot's url to the combination of its segments
                camurl = "http://" + GetIPsegment1 + "." + GetIPsegment2 + "." + GetIPsegment3 + "." + GetIPsegment5 + "/";  // Set the camera's url to the combination of its segments

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

        SafeMode.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()                                                   // Check if the Safe Mode switch has changed state
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)                                                     // When the state has changed
            {
                if (SafeMode.isChecked())                                                                                                  // If the Switch is on,
                {
                    SafeModeState = true;                                                                                                  // Set the safe mode state boolean as true.
                }
                else                                                                                                                       // If it isn't on,
                {
                    SafeModeState = false;                                                                                                 // Set the safe mode state boolean as false.
                }
            }
        });

        FlashSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()                                                // Check if the Flash switch has changed state
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)                                                     // When the state has changed
            {
                if (FlashSwitch.isChecked())                                                                                               // If the Switch is on,
                {
                    RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                // Create a new request queue
                    StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"FLASHON", new Response.Listener<String>()  // Make a request for the motor to stop
                    {
                        @Override
                        public void onResponse(String response)                                                                             // If there is a response from the server,
                        {

                        }
                    },
                        new Response.ErrorListener()                                                                                        // Check for errors
                        {
                           @Override
                           public void onErrorResponse(VolleyError error)                                                                   // If there is an error
                           {
                              Toast.makeText(MainActivity.this,"Could not be changed",Toast.LENGTH_SHORT).show();              // Warn the user that the motor couldn't be stopped because the robot is not connected.
                           }
                        }
                    );
                    queue.add(stringRequest);
                }
                else                                                                                                                         // If it isn't on,
                {
                    RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                  // Create a new request queue
                    StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"FLASHOFF", new Response.Listener<String>()  // Make a request for the motor to stop
                    {
                        @Override
                        public void onResponse(String response)                                                                              // If there is a response from the server,
                        {

                        }
                    },
                       new Response.ErrorListener()                                                                                          // Check for errors
                        {
                        @Override
                        public void onErrorResponse(VolleyError error)                                                                       // If there is an error
                            {
                                Toast.makeText(MainActivity.this,"Could not be changed",Toast.LENGTH_SHORT).show();             // Warn the user that the motor couldn't be stopped because the robot is not connected.
                            }
                        }
                    );
                    queue.add(stringRequest);
                }
            }
        });


        seekBar1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()                                                            // Check if the seekbar 1 has changed state
        {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)                                                   // If it has changed state
            {

                RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                      // Create a new request queue and
                StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"1/"+progress, new Response.Listener<String>()   // make a request with the progress value of the seekbar. Listen for a response.
                {
                    @Override
                    public void onResponse(String response)                                                                                   // If there is a response from the server
                    {
                                                                                                                                              // Do nothing
                    }

                },

                    new Response.ErrorListener()                                                                                              // Check for errors
                    {
                        @Override
                        public void onErrorResponse(VolleyError error)                                                                        // If there has been an error
                        {
                            Toast.makeText(MainActivity.this,"Robot is not connected.",Toast.LENGTH_LONG).show();                // Warn the user that the robot is not connected
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

        CamButton.setOnClickListener(new View.OnClickListener()                                                                                   // Listen for clicks in the take a picture button
        {
            @Override public void onClick(View v)                                                                                                 // When a click is registered
            {
                camurl = "http://" + GetIPsegment1 + "." + GetIPsegment2 + "." + GetIPsegment3 + "." + GetIPsegment5 + "/cam-lo.jpg";
                Camera.loadUrl(camurl);
            }
        });

    }



    @Override
    protected void onDestroy()                                                                                                                    // When the app is closed
    {
        super.onDestroy();

        RequestQueue queue = Volley.newRequestQueue(MainActivity.this);                                                                   // Create a new request queue and
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url+"STOP", new Response.Listener<String>()                       // make a request to turn of the motors. Listen for a response.
        {
            @Override
            public void onResponse(String response)                                                                                               // If there is a response from the server
            {
                                                                                                                                                  // Do nothing
            }

        },
             new Response.ErrorListener()                                                                                                         // Check for errors
             {
                 @Override
                 public void onErrorResponse(VolleyError error)                                                                                   // If there has been an error
                 {
                     Toast.makeText(MainActivity.this,"Robot is not connected, robot couldn't be stopped.",Toast.LENGTH_LONG).show();  // Warn the user that the robot couldn't be stopped because the robot is not connected.
                 }
             }
        );
        queue.add(stringRequest);                                                                                                                  // Add the request to the queue
         // Add the request to the queue
    }

}

