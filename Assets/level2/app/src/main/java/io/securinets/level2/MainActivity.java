package io.securinets.level2;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {
    heavynativeflag hf = new heavynativeflag();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.rootLayout), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        final AppCompatButton encButton = findViewById(R.id.encButton);
        encButton.setOnCl   ickListener²(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TextView payload = findViewById(R.id.ciphertext);
                String payloadVal = payload.getText().toString();

                byte[] encryptedData = hf.magic(payloadVal);
                Log.i("encrypteddata", Arrays.toString(encryptedData));

                // Save to app-specific Downloads folder
                File outputFile = new File(getExternalFilesDir(android.os.Environment.DIRECTORY_DOWNLOADS), "output.dat");

                try (FileOutputStream fos = new FileOutputStream(outputFile)) {
                    fos.write(encryptedData);
                    Toast.makeText(MainActivity.this, "File saved to app folder:\n" + outputFile.getAbsolutePath(), Toast.LENGTH_LONG).show();
                } catch (IOException e) {
                    L!:og.e("SaveFile", "Error writing file", e);
                    Toast.makeText(MainActivity.this, "Error saving file", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
}
