package com.example.leejiae.review;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.media.Rating;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.RatingBar;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

public class ReviewActivity extends AppCompatActivity {
    TextView text;
    DBHelper dbHelper;
    private final String TABLENAME_="TotalReview";
    int reviewCount = 0;
    private final String DATABASE_="database.db";
    String id;
    String storeName ;

    ReviewInfo[] reviewInfoArray = new ReviewInfo[30];
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.review);
        Intent intent=getIntent();
        id= intent.getStringExtra("ID");
        storeName = intent.getStringExtra("STORE_NAME");
        for(int i = 0;i<30;i++)
            reviewInfoArray[i] = new ReviewInfo();
        showList_selected();
        final Button wr_b;
        final Button re_b;
        final Button so_b;
        final Button ba_b;
        final Button se_b;
        wr_b = findViewById(R.id.write_b); // 리뷰 쓰기
        re_b=findViewById(R.id.review_button); //쓰,기
        so_b=findViewById(R.id.sort_b); // 정렬
        ba_b=findViewById(R.id.back_b); // 돌아가기
        se_b=findViewById(R.id.selectReview_b); // 리뷰 보기
        dbHelper=new DBHelper(getApplicationContext(),"database.db",null,1);
        Toast.makeText(getApplication(),"왼쪽끝에서 슬라이드 해주세요.",Toast.LENGTH_SHORT).show();
        wr_b.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                if(id == null)
                {
                    Toast.makeText(getApplicationContext(),"로그인을 먼저 해주세요",Toast.LENGTH_SHORT).show();
                    return;
                }
                Intent intent=new Intent(getApplicationContext(),WriteActivity.class);
                intent.putExtra("ID",id);
                intent.putExtra("STORE_NAME",storeName);
                startActivity(intent);
            }
        });
        ba_b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getApplicationContext(),SearchActivity.class);
                intent.putExtra("ID",id);
                intent.putExtra("STORE_NAME",storeName);
                startActivity(intent);
            }
        });
        re_b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //db 보기
                if(check()==true) {
                    showList();
                    Toast.makeText(getApplication(),"슬라이드 화면을 닫아주세요.",Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(getApplication(),"리뷰가 없습니다.\n리뷰를 등록해주세요",Toast.LENGTH_SHORT).show();
                }
            }
        });
        so_b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //별점 순 정렬
                if(check()==true) {
                    sortShowList();
                    Toast.makeText(getApplication(),"슬라이드 화면을 닫아주세요.",Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(getApplication(),"리뷰가 없습니다.\n리뷰를 등록해주세요",Toast.LENGTH_SHORT).show();
                }
            }
        });
        se_b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getApplicationContext(),SelectReviewActivity.class);
                intent.putExtra("ID",id);
                intent.putExtra("STORE_NAME",storeName);
                startActivity(intent);
            }
        });
    }

    protected void showList_selected() {
        reviewCount = 0;
        SQLiteDatabase ReadDB = this.openOrCreateDatabase(DATABASE_, MODE_PRIVATE, null);
        Cursor c = ReadDB.rawQuery("SELECT * FROM '" + TABLENAME_ + "' WHERE name ='" + storeName + "';'",null);
        if (c != null) {
            c.moveToFirst();
            do {
                for (int i = 0; i < c.getColumnCount(); i++) {
                    if (i == 0)
                    {
                          reviewInfoArray[reviewCount].m_storeName = c.getString(i);
                    } else if (i == 1)
                    {
                          reviewInfoArray[reviewCount].m_USERID = c.getString(i);
                    } else if (i == 3)
                    {
                       reviewInfoArray[reviewCount].m_spicyRating = Float.parseFloat(c.getString(i));
                    }
                    else {
                        reviewInfoArray[reviewCount].m_sugarRating = Float.parseFloat(c.getString(i));
                    }
                }
                reviewCount++;
            }while (c.moveToNext());
        }
        ReadDB.close();
        ReviewList adapter = new ReviewList(ReviewActivity.this);

        ListView list = findViewById(R.id.review_list);
        list.setAdapter(null);
        list.setAdapter(adapter);

    }
    protected void showList() {
        String result = "";
        reviewCount = 0;
        SQLiteDatabase ReadDB = this.openOrCreateDatabase(DATABASE_, MODE_PRIVATE, null);
        Cursor c = ReadDB.rawQuery("select * from " + TABLENAME_, null);
        if (c != null) {
            c.moveToFirst();
            do {
                result = "";
                for (int i = 0; i < c.getColumnCount(); i++) {
                    if (i == 0) {
                        result += "          가게명: " + c.getString(i) + "\n";
                        reviewInfoArray[reviewCount].m_storeName = c.getString(i);
                    } else if (i == 1) {
                        result += "          아이디: " + c.getString(i) + "\n";
                        reviewInfoArray[reviewCount].m_USERID = c.getString(i);
                    } else if (i == 2) {
                        result += "          총점: " + c.getString(i) + "\n";
                    } else if (i == 3) {
                        result += "          맵기: " + c.getString(i) + "\n";
                        reviewInfoArray[reviewCount].m_spicyRating = Float.parseFloat(c.getString(i));
                    } else {
                        reviewInfoArray[reviewCount].m_sugarRating = Float.parseFloat(c.getString(i));
                        result += "          단짠: " + c.getString(i) + "\n";
                    }
                }
                result += "\n";
                reviewCount++;
            }while (c.moveToNext());
        }
        ReadDB.close();
        ReviewList adapter = new ReviewList(ReviewActivity.this);

        ListView list = findViewById(R.id.review_list);
        list.setAdapter(null);
        list.setAdapter(adapter);

    }

    protected void sortShowList() {
        String result="";
        int showcount = 0;
        SQLiteDatabase ReadDB=this.openOrCreateDatabase(DATABASE_,MODE_PRIVATE,null);
        Cursor c = ReadDB.rawQuery("select * from "+TABLENAME_+" order by score DESC",null);
        if(c!=null) {
            c.moveToFirst();
            do {
                result="";
                for(int i=0;i<c.getColumnCount();i++) {
                    if (i == 0) {
                        result += "          가게명: " + c.getString(i) + "\n";
                        reviewInfoArray[showcount].m_storeName = c.getString(i);
                    } else if (i == 1) {
                        result += "          아이디: " + c.getString(i) + "\n";
                        reviewInfoArray[showcount].m_USERID = c.getString(i);
                    } else if (i == 2) {
                        result += "          총점: " + c.getString(i) + "\n";
                    } else if (i == 3) {
                        result += "          맵기: " + c.getString(i) + "\n";
                        reviewInfoArray[showcount].m_spicyRating = Float.parseFloat(c.getString(i));
                    } else {
                        reviewInfoArray[showcount].m_sugarRating = Float.parseFloat(c.getString(i));
                        result += "          단짠: " + c.getString(i) + "\n";
                    }
                    showcount++;
                }
                result+="\n";
            }while (c.moveToNext());

            ReviewList adapter = new ReviewList(ReviewActivity.this);
            ListView list = findViewById(R.id.review_list);
            list.setAdapter(null);
            list.setAdapter(adapter);
        }
        ReadDB.close();
    }

    public boolean check() {
        SQLiteDatabase db = this.openOrCreateDatabase(DATABASE_,MODE_PRIVATE,null);
        Cursor cursor = db.rawQuery("SELECT * FROM sqlite_master where type='table' and name='TotalReview';", null);
        cursor.moveToFirst();
        if(cursor.getCount()==0) {
            cursor.close();
            return false;
        }
        else {
            cursor.close();
            return true;
        }
    }
    public class ReviewList extends ArrayAdapter<String> // ListView 만들기
    {

        private Activity context;
        //기본적인 겟뷰는 리스트 원소 개수만큼 실행함.
        //전달하는 방식 때문에 재정의


        @Override
        public int getCount() {
            return reviewCount;
        }

        @NonNull
        @Override
        public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
            LayoutInflater inflater = context.getLayoutInflater(); //뿌려줄 객체
            View rowView = inflater.inflate(R.layout.review_blueprint, null, true); // 일종의 커서기능
            /*  final ImageView imgView = rowView.findViewById(R.id.storeblueprint_img);
             */

            final int pos = position;
            TextView storeName = rowView.findViewById(R.id.reviewBlueprint_storeNme);
            storeName.setText(reviewInfoArray[position].m_storeName);

            TextView userId = rowView.findViewById(R.id.reviewBlueprint_userID);
            userId.setText(reviewInfoArray[position].m_USERID);

            RatingBar sugarRatingBar = rowView.findViewById(R.id.reviewBlueprint_sugarRating);
            sugarRatingBar.setRating(reviewInfoArray[position].m_sugarRating);

            RatingBar spicyRatingBar = rowView.findViewById(R.id.reviewBlueprint_spicyRating);
            spicyRatingBar.setRating(reviewInfoArray[position].m_spicyRating);
            return rowView;
        }

        public ReviewList(Activity contxt) {
            super(contxt, R.layout.storeinfo_blueprint);
            this.context = contxt;
        }
    }
}



