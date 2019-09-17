package com.example.leejiae.review;

import android.view.Menu;

/**
 * Created by Ji-Young on 2018-10-31.
 */

class MenuInfo
{
    String m_menuName;
    String m_price;
}

class Location
{
    Float lat; //위도
    Float lng; //경도
}

public class StoreInfo {
    String m_storeName;
    MenuInfo[] m_menu;
    String m_category;
    String m_location_string;
    String m_tel;
    Location m_location;
    String m_openCloseInfo;
    String m_personalDayInfo;
    String m_storeUrl;
    String m_imageUrl;
    String m_storeFeatrue;
    StoreInfo()
    {
        m_location = new Location();

        m_menu = new MenuInfo[3];
        for(int i = 0;i<3;i++)
            m_menu[i] = new MenuInfo();
    }

    public void SetStroeInfo(StoreInfo temp)
    {
        m_storeName = temp.m_storeName;

    }

}