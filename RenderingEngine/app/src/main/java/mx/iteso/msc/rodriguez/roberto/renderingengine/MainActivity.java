package mx.iteso.msc.rodriguez.roberto.renderingengine;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;

import com.dropbox.client2.DropboxAPI;
import com.dropbox.client2.android.AndroidAuthSession;
import com.dropbox.client2.session.AppKeyPair;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    // Variables used to connect to Dropbox
    final static private String APP_KEY = "n9piube6a9p353k";
    final static private String APP_SECRET = "vq38jxpf771juij";
    private DropboxAPI<AndroidAuthSession> mDBApi;
    private Boolean result;

    // Navigation drawer
    @BindView(R.id.drawer_layout)
    DrawerLayout drawer;
    @BindView(R.id.nav_view)
    NavigationView navigation;
    @BindView(R.id.fab)
    FloatingActionButton fab;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Load the NDK library for OpenGL
        System.loadLibrary("RenderingEngine");

        // Bind the butter knife variables
        ButterKnife.bind(this);

        // Drawer toggle to add a listener to show/hide the floating button
        ActionBarDrawerToggle drawerToggle = new ActionBarDrawerToggle(this, drawer, R.string.drawer_open, R.string.drawer_close)
        {
            @Override
            public void onDrawerSlide(View drawerView, float slideOffset) {
                super.onDrawerSlide(drawerView, slideOffset);
                fab.setTranslationX(slideOffset * 200);
            }
        };
        // Listener to open the drawer with the floating button
        drawer.addDrawerListener(drawerToggle);
        // Listener to catch clicks on the drawer elements
        navigation.setNavigationItemSelectedListener(this);
    }

    @OnClick(R.id.fab)
    void openDrawer() {
        // Open the Drawer of the application
        drawer.openDrawer(navigation);
    }

    @Override
    public void onBackPressed() {
        if (drawer.isDrawerOpen(GravityCompat.START))
            drawer.closeDrawer(GravityCompat.START);
        else
            super.onBackPressed();
    }

    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        if(item.getItemId() == R.id.nav_local) {
            ((MainFragment)getFragmentManager().findFragmentById(R.id.fragment)).loadResources(true);
        }
        // Dropbox resources selected
        else {
            // TODO: Check if the validation was saved to avoid doing it again
            // Start the authentication with Dropbox
            AppKeyPair appKeys = new AppKeyPair(APP_KEY, APP_SECRET);
            // Creeate a session
            AndroidAuthSession session = new AndroidAuthSession(appKeys);
            mDBApi = new DropboxAPI<AndroidAuthSession>(session);
            // Start the authentication, this will open either the application of the browser
            mDBApi.getSession().startOAuth2Authentication(MainActivity.this);
        }
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mDBApi != null && mDBApi.getSession().authenticationSuccessful()) {
            try {
                // Required to complete auth, sets the access token on the session
                mDBApi.getSession().finishAuthentication();
                String accessToken = mDBApi.getSession().getOAuth2AccessToken();
                // TODO: save the access token to avoid the re-authentication at each application start
                // Get the list of files from Dropbox and download them
                GetFiles getFiles = new GetFiles(MainActivity.this, mDBApi, result);
                getFiles.execute();
                // Check the result of the process

                // Call the method on the fragment to load the new files
                ((MainFragment)getFragmentManager().findFragmentById(R.id.fragment)).loadResources(false);
            } catch (IllegalStateException e) {
                Log.i("DbAuthLog", "Error authenticating", e);
            }
        }
    }
}
