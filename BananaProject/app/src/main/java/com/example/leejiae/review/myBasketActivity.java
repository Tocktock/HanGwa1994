package com.example.leejiae.review;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Layout;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.maps.MapFragment;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import org.w3c.dom.Text;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;

public class myBasketActivity extends Activity {
    String USER_ID;
    String[][] sendArr;
    View popUpView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Toast.makeText(getApplication(), "뒤로 가시려면 바나나를 눌러주세요.", Toast.LENGTH_SHORT).show();
        final DBHelper dbHelper = new DBHelper(getApplicationContext(), "member.db", null, 1);
        final Intent intent = getIntent();
        USER_ID = intent.getStringExtra("ID");

        TextView a = findViewById(R.id.storeInfo_menu2);

        popUpView = getLayoutInflater().inflate(R.layout.storeinfo,null, false);
        sendArr=dbHelper.selectBasket(USER_ID);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_basket);

        //바나나 이미지 누르면 뒤로가기!!
        ImageView goMainActivity=(ImageView)findViewById(R.id.goMainAct);
        View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                switch (view.getId()) {
                    case R.id.goMainAct:
                        Intent goMain = new Intent(getApplicationContext(), MainActivity.class);
                        goMain.putExtra("ID",USER_ID);
                        startActivity(goMain);
                        break;
                }
            }
        };
        goMainActivity.setOnClickListener(listener);

        ZZimList adapter=new ZZimList(myBasketActivity.this);
        ListView store_List=findViewById(R.id.listBasket);
        store_List.setAdapter(adapter);
    }

    public class ZZimList extends ArrayAdapter<String>{
        private Activity context;
        @NonNull
        @Override
        public View getView(final int k,@NonNull View convertView, @NonNull ViewGroup parent){
            final DBHelper dbHelper = new DBHelper(getApplicationContext(), "member.db", null, 1);
            LayoutInflater inflater=context.getLayoutInflater();
            View rowView=inflater.inflate(R.layout.list_item,null,true);
            final TextView store_Name=rowView.findViewById(R.id.store_name);
            TextView store_URL=rowView.findViewById(R.id.goURL);
            final Button cancelZ=rowView.findViewById(R.id.cancelZZim);
            store_Name.setText("가게명: "+sendArr[0][k]);
            store_URL.setText(sendArr[1][k]);

            store_Name.setTag(k);
            store_URL.setTag(k);
            cancelZ.setTag(k);

            //여기는 지용햄이 채워주세요.
            //url은 sendArr[1][k]에 저장되어 있습니다.
            store_URL.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {

                    DetailCraweler detailCraweler = new DetailCraweler();
                    detailCraweler.SetURL(sendArr[1][k]);
                    detailCraweler.execute();

                }
            });

            cancelZ.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    int key=Integer.parseInt(cancelZ.getTag().toString());
                    dbHelper.deleteBasket(USER_ID,sendArr[0][key]);
                    Intent goBack = new Intent(getApplicationContext(), myBasketActivity.class);
                    goBack.putExtra("ID",USER_ID);
                    startActivity(goBack);
                }
            });

            return rowView;
        }

        public ZZimList(Activity contxt) {
            super(contxt, R.layout.list_item,sendArr[0]);
            this.context=contxt;
        }
    }
    MapFragment mapFragment;
    public class DetailCraweler extends AsyncTask
    {
        Document m_doc;
        Elements m_contents,m_detailContents,m_detailChildContents;
        int m_position;
        String m_url;
        StoreInfo m_storeInfo = new StoreInfo();
        public void SetURL(String url)
        {
            m_url = url;
        }

        @Override
        protected Object doInBackground(Object[] objects) {
            try {
                m_doc = Jsoup.connect(m_url).get();
                m_detailContents = m_doc.getElementsByAttributeValueContaining("id", "hdn_lat"); //위도
                m_storeInfo.m_location.lat = Float.parseFloat(m_detailContents.attr("value"));
                m_detailContents = m_doc.getElementsByAttributeValueContaining("id", "hdn_lng"); // 경도
                m_storeInfo.m_location.lng = Float.parseFloat(m_detailContents.attr("value"));
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "owner button");//위치 이름
                m_detailContents = m_detailContents.next();
                m_storeInfo.m_location_string = m_detailContents.text(); //위치 지번주소
                m_detailContents = m_detailContents.next();
                m_storeInfo.m_tel = m_detailContents.text(); // 전화번호
                m_detailContents = m_detailContents.next();
                //태그 할꺼면 하고
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "busi-hours short"); //영업시간
                m_detailContents = m_detailContents.select(".r-txt");
                int count_Time = 0;
                for (Element e : m_detailContents) {
                    if (count_Time > 0) //휴무 정보
                    {
                        m_storeInfo.m_personalDayInfo = e.text(); // 휴무일
                        continue;
                    }
                    m_storeInfo.m_openCloseInfo = e.text(); //영업 시간 이거
                    count_Time++;
                }
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "menu-info short");
                m_detailContents = m_detailContents.select(".list");
                m_detailChildContents = m_detailContents.select(".l-txt");

                int menuCount = 0;
                for (Element e : m_detailChildContents) //이름 넣는 부분
                {
                    if (menuCount > 2) //3개까지만 저장
                        break;
                    m_storeInfo.m_menu[menuCount].m_menuName = e.text(); //메뉴이름
                    menuCount++;
                }
                m_detailChildContents = m_detailContents.select(".r-txt");
                menuCount = 0;
                for (Element e : m_detailChildContents) // 가격 넣는 부분
                {
                    if (menuCount > 2) //3개까지만 저장
                        break;
                    m_storeInfo.m_menu[menuCount].m_price = e.text(); // 메뉴 가격
                    menuCount++;
                }
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "s-list pic-grade");
                m_detailChildContents = m_detailContents.select(".bimg img"); // 가게 이미지 src
                m_storeInfo.m_imageUrl = m_detailChildContents.attr("src");

            }
            catch (IOException e)
            {
                e.printStackTrace();
            }

            return null;
        }

        @Override
        protected void onPostExecute(Object o) {
            super.onPostExecute(o);
            final PopupWindow m_popupWindow;
            m_popupWindow = new PopupWindow(popUpView, LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
            m_popupWindow.setFocusable(true); // 외부영역 선택시 종료
            m_popupWindow.showAtLocation(popUpView, Gravity.CENTER, 0, 0);

            int mapId = popUpView.findViewById(R.id.storeInfo_map).getId();
            mapFragment = (MapFragment)getFragmentManager().findFragmentById(mapId);
            GoogleMapControl googleMap = new GoogleMapControl();
            googleMap.SetFragment(mapFragment);
            googleMap.SetLatLng(m_storeInfo.m_location.lat,m_storeInfo.m_location.lng,mapFragment);

            Button btnRegister, btnBack, btnShowReview;
            btnRegister = popUpView.findViewById(R.id.storeInfo_register);
            btnBack = popUpView.findViewById(R.id.storeInfo_backToMain);
            btnShowReview = popUpView.findViewById(R.id.storeInfo_showReview);

            TextView TVStoreName;
            TVStoreName = popUpView.findViewById(R.id.storeInfo_storeName);
            TVStoreName.setText(m_storeInfo.m_storeName);

            TextView TVStoreFeature;
            TVStoreFeature = popUpView.findViewById(R.id.storeInfo_storeFeature);
            TVStoreFeature.setText(m_storeInfo.m_storeFeatrue);

            TextView TVTel, TVOpenCloseTime, TVPersonalDay, TVLoactionString;
            TVTel = popUpView.findViewById(R.id.storeInfo_tel);
            TVOpenCloseTime = popUpView.findViewById(R.id.storeInfo_openCloseTime);
            TVPersonalDay = popUpView.findViewById(R.id.storeInfo_personalDay);
            TVLoactionString = popUpView.findViewById(R.id.storeInfo_locationString);
            TVTel.setText("전화 " + m_storeInfo.m_tel);

            if( m_storeInfo.m_openCloseInfo != null)
                TVOpenCloseTime.setText("영업시간 " + m_storeInfo.m_openCloseInfo);
            if( m_storeInfo.m_personalDayInfo != null)
                TVPersonalDay.setText("휴무일 " +m_storeInfo.m_personalDayInfo);

            TVLoactionString.setText("위치 "+ m_storeInfo.m_location_string);


            TextView[] TVMenu = new TextView[3];
            TextView[] TVPrice = new TextView[3];
            for (int i = 0; i < 3; i++) {
                int TVid = getResources().getIdentifier("storeInfo_menu" + (i + 1), "id", getPackageName()); //패키지 네임 수정 필요.
                int TVPriceID = getResources().getIdentifier("storeInfo_price" + (i + 1), "id", getPackageName());

                TVMenu[i] = popUpView.findViewById(TVid);
                TVMenu[i].setText(m_storeInfo.m_menu[i].m_menuName);
                TVPrice[i] = popUpView.findViewById(TVPriceID);
                TVPrice[i].setText(m_storeInfo.m_menu[i].m_price);
            }

            btnRegister.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if(USER_ID==null){
                        Toast.makeText(getApplication(), "로그인부터 해주세요.", Toast.LENGTH_SHORT).show();
                    }
                    else{
                        DBHelper dbHelper = new DBHelper(getApplicationContext(), "member.db", null, 1);
                        int curPoint=dbHelper.getCursorPoint(USER_ID);
                        boolean check=dbHelper.selectStoreName(USER_ID,m_storeInfo.m_storeName);
                        if(!check){
                            if(curPoint<5){
                                dbHelper.insertBasket(USER_ID,m_storeInfo.m_storeName, m_storeInfo.m_storeUrl);
                                Toast.makeText(getApplication(), USER_ID + "님 찜목록 추가하였습니다.", Toast.LENGTH_SHORT).show();
                            }
                            else{
                                Toast.makeText(getApplication(), "찜은 5개까지 가능합니다.", Toast.LENGTH_SHORT).show();
                            }
                        }
                        else{
                            Toast.makeText(getApplication(), "이미 찜했습니다!!", Toast.LENGTH_SHORT).show();
                        }
                    }
                }
            });

            btnBack.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    m_popupWindow.dismiss();
                }
            });

            btnShowReview.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(getApplicationContext(), ReviewActivity.class);
                    intent.putExtra("ID",USER_ID);
                    intent.putExtra("STORE_NAME",m_storeInfo.m_storeName);
                    startActivity(intent);
                }
            });
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();


        }
    }
}
