package com.example.leejiae.review;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Fragment;
import android.content.Context;
import android.content.IntentSender;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.widget.ImageViewCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.content.Intent;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.ProgressBar;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;


import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.Date;
import java.util.logging.Logger;

public class SearchActivity extends Activity {

    TextView TVtemp;
    StoreInfo[] storeInfoArray = new StoreInfo[30];
    Integer storeCount = 0;

    class ShowControl implements CompoundButton.OnCheckedChangeListener {
        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            String str = '*' + buttonView.getText().toString();
            for (int i = 0; i < 4; i++) //총 이미지뷰 개수만큼 비교.
            {

               /* TVtemp = findViewById(R.id.category_res1 + i);
                if(isChecked == true)
                {
                    if(TVtemp.getText().toString().equals(str))
                    {
                        tableRow = findViewById(R.id.res1 + i);
                        tableRow.setVisibility(View.VISIBLE);
                    }
                }
                else
                {
                    if( TVtemp.getText().toString().equals(str))
                    {
                        Toast.makeText(getApplicationContext(),TVtemp.getText().toString(),Toast.LENGTH_SHORT).show();
                        tableRow = findViewById(R.id.res1 + i);
                        tableRow.setVisibility(View.GONE);
                    }
                }*/
            }
        }
    }

    Button btn_WorldCup, btn_setting, btn_sch;
    EditText Edit_sch;
    String url_Crawl, keyWord_Crawl, key_Crawl, match_Crawl;
    String test;
    String USER_ID;
    View popUpView;
    ProgressBar progressBar;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Typeface type = Typeface.createFromAsset(getAssets(),"font/Binggrae-Bold.ttf");
        setContentView(R.layout.activity_search);
        btn_sch = findViewById(R.id.btn_sch);
        popUpView = getLayoutInflater().inflate(R.layout.storeinfo, null, false);

        //크롤링 부분
        btn_sch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Edit_sch = findViewById(R.id.search);
                String schStr = Edit_sch.getText().toString();
                Crawler crawler = new Crawler();
                crawler.SetElementsInfo("https://www.diningcode.com/list.php?query=", schStr, "class", "blink");
                crawler.execute();
            }

        });
        progressBar = findViewById(R.id.search_progressBar);

        btn_WorldCup = findViewById(R.id.btn_worldCup);
        btn_WorldCup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), WorldCupActivity.class);
                startActivity(intent);
            }
        });
        btn_setting = findViewById(R.id.main_btn_setting);
        btn_setting.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                intent.putExtra("ID",USER_ID);
                startActivity(intent);
            }
        });

    }


    @Override
    protected void onStart() {
        super.onStart();
        Intent intent = getIntent();
        USER_ID = intent.getStringExtra("ID");
        String time = getIntent().getStringExtra("time");
        TVtemp = findViewById(R.id.tv_goTime);

        if (time == null) {
            TVtemp.setText("설정안됨");
        } else {
            TVtemp.setText(time);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    public class Crawler extends AsyncTask {

        Elements m_contents, m_detailContents, m_detailChildContents;
        Document m_doc = null, m_childDoc = null;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            progressBar.setVisibility(View.VISIBLE);

        }

        public void SetElementsInfo(String url, String keyWord, String key, String match) {
            url_Crawl = url;
            key_Crawl = key;
            match_Crawl = match;
            keyWord_Crawl = keyWord;
        }

        @Override
        protected Object doInBackground(Object[] objects) {
            try {
                storeCount = 0;
                int count = 0;
                String encodedUrl = URLEncoder.encode(keyWord_Crawl, "utf-8");
                m_doc = Jsoup.connect("https://www.diningcode.com/list.php?query=" + encodedUrl).get();
                m_contents = m_doc.getElementsByAttributeValueContaining(key_Crawl, match_Crawl);

                for (Element content : m_contents) {
                    if (count == 0) //첫번째 애는 광고기 때문에 제외
                    {
                        count++;
                        continue;
                    }
                    String detailLink = content.attr("href");
                    StoreInfo storeInfo = new StoreInfo();
                    storeInfo.m_storeName = content.select(".btxt").text();
                    storeInfo.m_storeName = storeInfo.m_storeName.substring(2);
                    storeInfo.m_storeUrl = "https://www.diningcode.com/" + detailLink; //가게 url
                    m_detailChildContents = content.select(".stxt");
                    storeInfo.m_storeFeatrue = m_detailChildContents.text();

                    storeInfoArray[storeCount] = new StoreInfo();
                    storeInfoArray[storeCount] = storeInfo;
                    storeCount++; //얘는 다시 검색할 때 초기화 해야함.
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Object o) {
            super.onPostExecute(o);
            StoreList adapter;
            progressBar.setVisibility(View.GONE);
            synchronized (this) {
                adapter = new StoreList(SearchActivity.this);
            }
            ListView list = findViewById(R.id.main_storeList);
            list.setAdapter(null);
            list.setAdapter(adapter);
            list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                    DetailCraweler detailCraweler = new DetailCraweler();
                    detailCraweler.SetPosition(position);
                    detailCraweler.execute();

                }
            });
        }
    }

    public class StoreList extends ArrayAdapter<String> // ListView 만들기
    {

        private Activity context;
        //기본적인 겟뷰는 리스트 원소 개수만큼 실행함.
        //전달하는 방식 때문에 재정의


        @Override
        public int getCount() {

            return storeCount;
        }

        @NonNull
        @Override
        public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
            LayoutInflater inflater = context.getLayoutInflater(); //뿌려줄 객체
            View rowView = inflater.inflate(R.layout.storeinfo_blueprint, null, true); // 일종의 커서기능
            /*  final ImageView imgView = rowView.findViewById(R.id.storeblueprint_img);
             */
            TextView feature;
            final int pos = position;
            TextView storeName = rowView.findViewById(R.id.storeblueprint_storeName);
            storeName.setText(storeInfoArray[position].m_storeName);
            feature = rowView.findViewById(R.id.storeblueprint_feature);
            feature.setText(storeInfoArray[position].m_storeFeatrue);
            return rowView;
        }

        public StoreList(Activity contxt) {
            super(contxt, R.layout.storeinfo_blueprint);
            this.context = contxt;
        }
    }


    MapFragment mapFragment = new MapFragment();

    public class DetailCraweler extends AsyncTask
    {
        Document m_doc;
        Elements m_contents,m_detailContents,m_detailChildContents;
        int m_position;
        public void SetPosition(int position)
        {
            m_position = position;
        }

        @Override
        protected Object doInBackground(Object[] objects) {
            try {
                StoreInfo storeInfo;
                storeInfo = storeInfoArray[m_position];
                m_doc = Jsoup.connect(storeInfo.m_storeUrl).get();
                m_detailContents = m_doc.getElementsByAttributeValueContaining("id", "hdn_lat"); //위도
                storeInfo.m_location.lat = Float.parseFloat(m_detailContents.attr("value"));
                m_detailContents = m_doc.getElementsByAttributeValueContaining("id", "hdn_lng"); // 경도
                storeInfo.m_location.lng = Float.parseFloat(m_detailContents.attr("value"));
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "owner button");//위치 이름
                m_detailContents = m_detailContents.next();
                storeInfo.m_location_string = m_detailContents.text(); //위치 지번주소
                m_detailContents = m_detailContents.next();
                storeInfo.m_tel = m_detailContents.text(); // 전화번호
                m_detailContents = m_detailContents.next();
                //태그 할꺼면 하고
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "busi-hours short"); //영업시간
                m_detailContents = m_detailContents.select(".r-txt");
                int count_Time = 0;
                for (Element e : m_detailContents) {
                    if (count_Time > 0) //휴무 정보
                    {
                        storeInfo.m_personalDayInfo = e.text(); // 휴무일
                        continue;
                    }
                    storeInfo.m_openCloseInfo = e.text(); //영업 시간 이거
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
                    storeInfo.m_menu[menuCount].m_menuName = e.text(); //메뉴이름
                    menuCount++;
                }
                m_detailChildContents = m_detailContents.select(".r-txt");
                menuCount = 0;
                for (Element e : m_detailChildContents) // 가격 넣는 부분
                {
                    if (menuCount > 2) //3개까지만 저장
                        break;
                    storeInfo.m_menu[menuCount].m_price = e.text(); // 메뉴 가격
                    menuCount++;
                }
                m_detailContents = m_doc.getElementsByAttributeValueContaining("class", "s-list pic-grade");
                m_detailChildContents = m_detailContents.select(".bimg img"); // 가게 이미지 src
                storeInfo.m_imageUrl = m_detailChildContents.attr("src");

                storeInfoArray[m_position] = storeInfo;
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
            googleMap.SetLatLng(storeInfoArray[m_position].m_location.lat,storeInfoArray[m_position].m_location.lng,mapFragment);

            Button btnRegister, btnBack, btnShowReview;
            btnRegister = popUpView.findViewById(R.id.storeInfo_register);
            btnBack = popUpView.findViewById(R.id.storeInfo_backToMain);
            btnShowReview = popUpView.findViewById(R.id.storeInfo_showReview);

            TextView TVStoreName;
            TVStoreName = popUpView.findViewById(R.id.storeInfo_storeName);
            TVStoreName.setText(storeInfoArray[m_position].m_storeName);

            TextView TVStoreFeature;
            TVStoreFeature = popUpView.findViewById(R.id.storeInfo_storeFeature);
            TVStoreFeature.setText(storeInfoArray[m_position].m_storeFeatrue);

            TextView TVTel, TVOpenCloseTime, TVPersonalDay, TVLoactionString;
            TVTel = popUpView.findViewById(R.id.storeInfo_tel);
            TVOpenCloseTime = popUpView.findViewById(R.id.storeInfo_openCloseTime);
            TVPersonalDay = popUpView.findViewById(R.id.storeInfo_personalDay);
            TVLoactionString = popUpView.findViewById(R.id.storeInfo_locationString);
            TVTel.setText("전화 " + storeInfoArray[m_position].m_tel);

            if( storeInfoArray[m_position].m_openCloseInfo != null)
                TVOpenCloseTime.setText("영업시간 " + storeInfoArray[m_position].m_openCloseInfo);
            if( storeInfoArray[m_position].m_personalDayInfo != null)
                TVPersonalDay.setText("휴무일 " + storeInfoArray[m_position].m_personalDayInfo);

            TVLoactionString.setText("위치 "+ storeInfoArray[m_position].m_location_string);

            TextView[] TVMenu = new TextView[3];
            TextView[] TVPrice = new TextView[3];
            for (int i = 0; i < 3; i++) {
                int TVid = getResources().getIdentifier("storeInfo_menu" + (i + 1), "id", getPackageName()); //패키지 네임 수정 필요.
                int TVPriceID = getResources().getIdentifier("storeInfo_price" + (i + 1), "id", getPackageName());

                TVMenu[i] = popUpView.findViewById(TVid);
                TVMenu[i].setText(storeInfoArray[m_position].m_menu[i].m_menuName);
                TVPrice[i] = popUpView.findViewById(TVPriceID);
                TVPrice[i].setText(storeInfoArray[m_position].m_menu[i].m_price);
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
                        boolean check=dbHelper.selectStoreName(USER_ID,storeInfoArray[m_position].m_storeName);
                        if(!check){
                            if(curPoint<5){
                                dbHelper.insertBasket(USER_ID,storeInfoArray[m_position].m_storeName, storeInfoArray[m_position].m_storeUrl);
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
                    intent.putExtra("STORE_NAME",storeInfoArray[m_position].m_storeName);
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


