package mx.iteso.msc.rodriguez.roberto.renderingengine;

import android.app.Fragment;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

/**
 * Created by Roberto Rodriguez on 5/12/2016.
 */
public class MainFragment extends Fragment {
    // Surface for OpenGL ES 2.0
    private GLSurfaceView mGLView;
    private View view;

    private boolean mRunning = false;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_content, container, false);
        return view;
    }

    public void loadResources(boolean local) {
        // Check if cleaning is needed
        if (mRunning)
            // Call Shutdown on NDK code
            Shutdown();
        else
            mRunning = true;
        // Load local resources
        if (local) {
            // Remove the old view from the fragment
            ViewGroup parent = (ViewGroup)view.getParent();
            parent.removeView(view);
            // Create a new surface for OpenGL
            mGLView = new CustomGLSurfaceView(getActivity());
            // Put the surface on the fragment
            view = mGLView;
            parent.addView(view);
        }
        // Load the resources from Dropbox
        else {
        }
    }

    public native void Shutdown();
}
