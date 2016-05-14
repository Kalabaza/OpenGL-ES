package mx.iteso.msc.rodriguez.roberto.renderingengine;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.View;

/**
 * Created by Roberto Rodriguez on 5/7/2016.
 */
public class CustomGLSurfaceView extends GLSurfaceView {
    // Renderer for OpenGL
    private final CustomGLRenderer mRenderer;

    public CustomGLSurfaceView(Context context) {
        super(context);

        // Set OpenGL ES context to be version 2.0
        setEGLContextClientVersion(2);

        // Create the custom renderer
        mRenderer = new CustomGLRenderer(context);

        // Renderer for drawing on the OpenGL surface
        setRenderer(mRenderer);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int pointerCount = event.getPointerCount();
        boolean zoom = false;
        if (pointerCount > 1)
            zoom = true;

        float x = event.getX();
        float y = event.getY();

        switch (event.getAction()) {
            case MotionEvent.ACTION_MOVE: {
                x = event.getX();
                y = event.getY();
            }
                break;
        }
        return true;
    }

    public native void Update();
}
