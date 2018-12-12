// Original Source Code: https://github.com/shardul/Android

package com.ant.ant_manager.view.remoteui;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.text.method.ScrollingMovementMethod;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.ant.ant_manager.R;

public class RemoteNotiPopupView {

    protected WindowManager mWindowManager;

    protected Context mContext;
    protected PopupWindow mWindow;

    private TextView mHelpTextView;
    private ImageView mUpImageView;
    private ImageView mDownImageView;
    private ImageView mPopupContentsImageView;
    protected View mView;

    protected Drawable mBackgroundDrawable = null;
    protected ShowListener showListener;

    public RemoteNotiPopupView(Context context, int viewResource) {
        mContext = context;
        mWindow = new PopupWindow(context);

        mWindowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);

        LayoutInflater layoutInflater = (LayoutInflater) context.getSystemService(Context
                .LAYOUT_INFLATER_SERVICE);

        setContentView(layoutInflater.inflate(viewResource, null));

        mHelpTextView = (TextView) mView.findViewById(R.id.text);
        mUpImageView = (ImageView) mView.findViewById(R.id.popup_arrow_up);
        mDownImageView = (ImageView) mView.findViewById(R.id.popup_arrow_down);
        mPopupContentsImageView = (ImageView) mView.findViewById(R.id.popup_contents_image_view);
        mPopupContentsImageView.setAdjustViewBounds(true);
        mPopupContentsImageView.setMaxWidth(400);

        mHelpTextView.setMovementMethod(ScrollingMovementMethod.getInstance());
        mHelpTextView.setSelected(true);
    }

    public RemoteNotiPopupView(Context context) {
        this(context, R.layout.popup);
    }

    public RemoteNotiPopupView(Context context, String text, String imageFilePath) {
        this(context);

        setText(text);
        if (imageFilePath != null && !imageFilePath.isEmpty()) {
            setImage(imageFilePath);
        }
    }

    public void show(View anchor) {
        preShow();

        int[] location = new int[2];

        anchor.getLocationOnScreen(location);

        Rect anchorRect = new Rect(location[0], location[1], location[0] + anchor.getWidth(),
                location[1] + anchor.getHeight());

        mView.measure(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);

        int rootHeight = mView.getMeasuredHeight();
        int rootWidth = mView.getMeasuredWidth();

        final int screenWidth = mWindowManager.getDefaultDisplay().getWidth();
        final int screenHeight = mWindowManager.getDefaultDisplay().getHeight();

        int yPos = anchorRect.top - rootHeight;

        boolean onTop = true;

        if (anchorRect.top < screenHeight / 2) {
            yPos = anchorRect.bottom;
            onTop = false;
        }

        int whichArrow, requestedX;

        whichArrow = ((onTop) ? R.id.popup_arrow_down : R.id.popup_arrow_up);
        requestedX = anchorRect.centerX();

        View arrow = whichArrow == R.id.popup_arrow_up ? mUpImageView : mDownImageView;
        View hideArrow = whichArrow == R.id.popup_arrow_up ? mDownImageView : mUpImageView;

        final int arrowWidth = arrow.getMeasuredWidth();

        arrow.setVisibility(View.VISIBLE);

        ViewGroup.MarginLayoutParams param = (ViewGroup.MarginLayoutParams) arrow.getLayoutParams();

        hideArrow.setVisibility(View.INVISIBLE);

        int xPos = 0;

        // ETXTREME RIGHT CLIKED
        if (anchorRect.left + rootWidth > screenWidth) {
            xPos = (screenWidth - rootWidth);
        }
        // ETXTREME LEFT CLIKED
        else if (anchorRect.left - (rootWidth / 2) < 0) {
            xPos = anchorRect.left;
        }
        // INBETWEEN
        else {
            xPos = (anchorRect.centerX() - (rootWidth / 2));
        }

        param.leftMargin = (requestedX - xPos) - (arrowWidth / 2);

        if (onTop) {
            mHelpTextView.setMaxHeight(anchorRect.top - anchorRect.height());

        } else {
            mHelpTextView.setMaxHeight(screenHeight - yPos);
        }

        mWindow.showAtLocation(anchor, Gravity.NO_GRAVITY, xPos, yPos);

        mView.setAnimation(AnimationUtils.loadAnimation(mContext, R.anim.float_anim));

    }

    protected void preShow() {
        if (mView == null) throw new IllegalStateException("view undefined");


        if (showListener != null) {
            showListener.onPreShow();
            showListener.onShow();
        }

        if (mBackgroundDrawable == null) mWindow.setBackgroundDrawable(new BitmapDrawable());
        else mWindow.setBackgroundDrawable(mBackgroundDrawable);

        mWindow.setWidth(WindowManager.LayoutParams.WRAP_CONTENT);
        mWindow.setHeight(WindowManager.LayoutParams.WRAP_CONTENT);
        mWindow.setTouchable(true);
        mWindow.setFocusable(true);
        mWindow.setOutsideTouchable(true);

        mWindow.setContentView(mView);
    }

    public void setBackgroundDrawable(Drawable background) {
        mBackgroundDrawable = background;
    }

    public void setContentView(View root) {
        mView = root;

        mWindow.setContentView(root);
    }

    public void setContentView(int layoutResID) {
        LayoutInflater inflator = (LayoutInflater) mContext.getSystemService(Context
                .LAYOUT_INFLATER_SERVICE);

        setContentView(inflator.inflate(layoutResID, null));
    }

    public void setOnDismissListener(PopupWindow.OnDismissListener listener) {
        mWindow.setOnDismissListener(listener);
    }

    public void dismiss() {
        mWindow.dismiss();
        if (showListener != null) {
            showListener.onDismiss();
        }
    }

    public void setText(String text) {
        mHelpTextView.setText(text);
    }

    public void setImage(String imageFilePath) {
        if (imageFilePath != null && !imageFilePath.isEmpty()) {
            mPopupContentsImageView.setVisibility(View.VISIBLE);
            mPopupContentsImageView.setImageBitmap(BitmapFactory.decodeFile(imageFilePath));
        } else {
            mPopupContentsImageView.setVisibility(View.GONE);
        }
    }

    public static interface ShowListener {
        void onPreShow();

        void onDismiss();

        void onShow();
    }

    public void setShowListener(ShowListener showListener) {
        this.showListener = showListener;
    }
}
