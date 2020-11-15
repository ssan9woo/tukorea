package com.example.listview_test1;

import java.text.Collator;
import java.util.Comparator;

import android.graphics.drawable.Drawable;

public class ListData {
    /**
     * ����Ʈ ������ ��� ���� ��ü ����
     */
    // ������
    public Drawable mIcon;

    // ����
    public String mTitle;

    // ��¥
    public String mDate;

    /**
     * ���ĺ� �̸����� ����
     */
    public static final Comparator<ListData> ALPHA_COMPARATOR = new Comparator<ListData>() {
        private final Collator sCollator = Collator.getInstance();

        @Override
        public int compare(ListData mListDate_1, ListData mListDate_2) {
            return sCollator.compare(mListDate_1.mTitle, mListDate_2.mTitle);
        }
    };
}

