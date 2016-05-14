package mx.iteso.msc.rodriguez.roberto.renderingengine;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.AsyncTask;
import android.os.Environment;
import android.widget.Toast;

import com.dropbox.client2.DropboxAPI;
import com.dropbox.client2.DropboxAPI.Entry;
import com.dropbox.client2.exception.DropboxException;
import com.dropbox.client2.exception.DropboxIOException;
import com.dropbox.client2.exception.DropboxParseException;
import com.dropbox.client2.exception.DropboxPartialFileException;
import com.dropbox.client2.exception.DropboxServerException;
import com.dropbox.client2.exception.DropboxUnlinkedException;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;

/**
 * Created by Roberto Rodriguez on 5/13/2016.
 */
public class GetFiles extends AsyncTask<Void, Long, Boolean> {
    private Context mContext;
    private final ProgressDialog mDialog;
    private DropboxAPI<?> mApi;
    private Long mTotalLen = 0l;
    private Long mCurLen = 0l;
    private Boolean result;
    private DropboxAPI<?> mDBApi;
    private String mErrorMsg;
    private boolean mCanceled;
    private OutputStream out;

    public GetFiles(Context context, DropboxAPI<?> dbApi, Boolean sucess) {
        mContext = context.getApplicationContext();
        mDBApi = dbApi;
        result = sucess;

        mDialog = new ProgressDialog(context);
        mDialog.setMessage("Downloading resources from Dropbox...");
        mDialog.setButton(ProgressDialog.BUTTON_POSITIVE, "Cancel", new OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                mCanceled = true;
                mErrorMsg = "Canceled";

                // This will cancel the download of files
                if (out != null) {
                    try {
                        out.close();
                    } catch (IOException e) {
                    }
                }
            }
        });
        mDialog.show();
    }

    @Override
    protected Boolean doInBackground(Void... params) {
        // Get the directory listing using the metadata method
        try {
            if (mCanceled)
                return false;

            String path = "/RenderingEngine";
            String[] fileNames;
            Long[] fileLengths;
            ArrayList<String> fNames = new ArrayList<String>();
            ArrayList<Long> fLengths = new ArrayList<Long>();
            // Search for files on the Dropbox folder
            Entry existingEntries = mDBApi.metadata(path, 0, null, true, null);
            // Get the file names and sizes from the metadata
            for (Entry entry : existingEntries.contents) {
                if (!entry.isDir) {
                    // Get the file name
                    fNames.add(entry.fileName());
                    // Get the file size
                    fLengths.add(entry.bytes);
                    mTotalLen += entry.bytes;
                    if (mCanceled)
                        return false;
                }
            }
            fileNames = fNames.toArray(new String[fNames.size()]);
            fileLengths = fLengths.toArray(new Long[fLengths.size()]);
            // Clean the local directory to receive the new files
            File localFolder = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS) + path);
            if (!localFolder.exists())
                localFolder.mkdir();
            else {
                for (File child : localFolder.listFiles())
                    child.delete();
            }
            if (mCanceled)
                return false;
            // Download the new files to the local folder
            for (int index = 0; index < fileNames.length; ++index) {
                out = new BufferedOutputStream(new FileOutputStream(new File(localFolder + "/" + fileNames[index])));
                mDBApi.getFile(path + "/" + fileNames[index], null, out, null);
                mCurLen += fileLengths[index];
                publishProgress(mCurLen);
                if (mCanceled)
                    return false;
            }
            result = true;
        } catch (DropboxUnlinkedException e) {
            // The AuthSession wasn't properly authenticated or user unlinked.
        } catch (DropboxPartialFileException e) {
            // We canceled the operation
            mErrorMsg = "Download canceled";
        } catch (DropboxServerException e) {
            // Server-side exception.  These are examples of what could happen,
            // but we don't do anything special with them here.
            if (e.error == DropboxServerException._304_NOT_MODIFIED) {
                // won't happen since we don't pass in revision with metadata
            } else if (e.error == DropboxServerException._401_UNAUTHORIZED) {
                // Unauthorized, so we should unlink them.  You may want to
                // automatically log the user out in this case.
            } else if (e.error == DropboxServerException._403_FORBIDDEN) {
                // Not allowed to access this
            } else if (e.error == DropboxServerException._404_NOT_FOUND) {
                // path not found (or if it was the thumbnail, can't be
                // thumbnailed)
            } else if (e.error == DropboxServerException._406_NOT_ACCEPTABLE) {
                // too many entries to return
            } else if (e.error == DropboxServerException._415_UNSUPPORTED_MEDIA) {
                // can't be thumbnailed
            } else if (e.error == DropboxServerException._507_INSUFFICIENT_STORAGE) {
                // user is over quota
            } else {
                // Something else
            }
            // This gets the Dropbox error, translated into the user's language
            mErrorMsg = e.body.userError;
            if (mErrorMsg == null) {
                mErrorMsg = e.body.error;
            }
        } catch (DropboxIOException e) {
            // Happens all the time, probably want to retry automatically.
            mErrorMsg = "Network error. Try again.";
        } catch (DropboxParseException e) {
            // Probably due to Dropbox server restarting, should retry
            mErrorMsg = "Dropbox error. Try again.";
        } catch (DropboxException e) {
            // Unknown error
            mErrorMsg = "Unknown error. Try again.";
        } catch (FileNotFoundException e) {
            mErrorMsg = "File error. Try again.";
        }
        return result;
    }

    @Override
    protected void onProgressUpdate(Long... progress) {
        int percent = (int)(100.0*(double)progress[0]/ mTotalLen + 0.5);
        mDialog.setProgress(percent);
    }

    @Override
    protected void onPostExecute(Boolean success) {
        mDialog.dismiss();
        if(success)
            showToast("Resources downloaded correctly");
        else
            showToast(mErrorMsg);
    }

    private void showToast(String msg) {
        Toast error = Toast.makeText(mContext, msg, Toast.LENGTH_LONG);
        error.show();
    }
}
