package mx.iteso.msc.rodriguez.roberto.renderingengine;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by Roberto Rodriguez on 5/7/2016.
 */
public class CustomGLRenderer implements GLSurfaceView.Renderer {
    static AssetManager sAssetManager;
    static Context sContext;
    static int width, height;

    public CustomGLRenderer(Context context) {
        sContext = context;
        sAssetManager = sContext.getAssets();
    }

    // Called once to set up the view's OpenGL ES environment.
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Display display = ((WindowManager)sContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);
        width = metrics.widthPixels;
        height = metrics.heightPixels;
        Init(sAssetManager, width, height);
    }

    // Called if the geometry of the view changes, for example when the device's screen orientation changes.
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        ;
    }

    // Called for each redraw of the view.
    @Override
    public void onDrawFrame(GL10 gl) {
        Draw(width, height);
    }

    public native void Init(AssetManager assetManager, int width, int height);
    public native void Draw(int width, int height);
}
