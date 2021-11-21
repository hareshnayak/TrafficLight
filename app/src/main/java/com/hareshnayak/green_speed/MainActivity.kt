package com.hareshnayak.green_speed

import android.os.Bundle
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener


class MainActivity : AppCompatActivity() {

    private var firebaseDatabase: FirebaseDatabase?=null
    lateinit var durationRed : TextView
    lateinit var durationYellow : TextView
    lateinit var durationGreen : TextView
    lateinit var distanceLeft : TextView
    lateinit var currentSpeed : TextView
    private lateinit var requiredSpeed : TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        durationRed = findViewById(R.id.duration_red)
        durationYellow = findViewById(R.id.duration_yellow)
        durationGreen = findViewById(R.id.duration_green)
        currentSpeed = findViewById(R.id.current_speed)
        distanceLeft = findViewById(R.id.distance_left)
        requiredSpeed = findViewById(R.id.reqd_speed)

        val secs: String = getString(R.string.secs)
        val cms: String = getString(R.string.cm_s)
        val dist: String = getString(R.string.dist_unit)


        firebaseDatabase = FirebaseDatabase.getInstance()

        val databaseReference = FirebaseDatabase.getInstance().reference
        databaseReference.addValueEventListener(object: ValueEventListener{
            var rs : Float = 0.0F
            override fun onDataChange(snapshot: DataSnapshot) {
                val model = arrayListOf<String>()
                for(data in snapshot.children){
                    model.add(data.value.toString())
                }
                currentSpeed.text = String.format(model[0],cms)
                durationGreen.text = String.format(model[3],secs)
                durationRed.text = String.format(model[4],secs)
                durationYellow.text = String.format(model[5],secs)
                distanceLeft.text = String.format(model[2],dist)
                rs = if(model[3].toInt() !=0){
                    (model[2].toInt()/model[3].toInt()).toFloat()
                } else model[2].toFloat()
                requiredSpeed.text = (rs).toString()
            }
            override fun onCancelled(error: DatabaseError) {
                Toast.makeText(this@MainActivity, error.code, Toast.LENGTH_SHORT).show()
            }
        })
    }
}