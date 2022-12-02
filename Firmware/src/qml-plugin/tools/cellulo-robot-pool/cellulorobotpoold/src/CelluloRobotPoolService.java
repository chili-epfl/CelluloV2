package ch.epfl.chili.cellulo.cellulorobotpoold;

import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.app.*;
import android.support.v4.app.NotificationCompat;
import org.qtproject.qt5.android.bindings.QtService;

public class CelluloRobotPoolService extends QtService {

    public static final int NOTIFICATION_ID = 2556;

    public static void startCelluloRobotPoolService(Context ctx){
        ctx.startService(new Intent(ctx, ch.epfl.chili.cellulo.cellulorobotpoold.CelluloRobotPoolService.class));
    }

    public static void stopCelluloRobotPoolService(Context ctx){
        ctx.stopService(new Intent(ctx, ch.epfl.chili.cellulo.cellulorobotpoold.CelluloRobotPoolService.class));
    }

    @Override
    public void onCreate(){
        NotificationCompat.Builder builder;
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
            builder = new NotificationCompat.Builder(this, "cellulorobotpoold");
        else
            builder = new NotificationCompat.Builder(this);
        builder.setSmallIcon(R.drawable.icon);
        builder.setContentTitle("Cellulo Robot Pool Service");
        builder.setContentText("Cellulo robot pool service running...");
        Notification notification = builder.build();
        NotificationManager notificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            NotificationChannel channel = new NotificationChannel("cellulorobotpoold", "Cellulo Robot Pool Service", NotificationManager.IMPORTANCE_DEFAULT);
            notificationManager.createNotificationChannel(channel);
        }
        notificationManager.notify(NOTIFICATION_ID, notification);
        startForeground(NOTIFICATION_ID, notification);

        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int ret = super.onStartCommand(intent, flags, startId);
        return ret;
    }

    @Override
    public IBinder onBind(Intent intent) {
        IBinder ret = super.onBind(intent);
        return ret;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        boolean ret = super.onUnbind(intent);
        return ret;
    }

    @Override
    public void onRebind(Intent intent) {
        super.onRebind(intent);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
