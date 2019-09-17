package com.example.leejiae.review;

import android.app.Activity;
import android.app.FragmentManager;
import android.os.Bundle;

import com.example.leejiae.review.R;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
public class GoogleMapControl extends Activity implements OnMapReadyCallback {

    Float m_lng;
    Float m_lat;
    MapFragment m_mapFragment;

    @Override
    public void onMapReady(final GoogleMap map) {
        LatLng locat = new LatLng(m_lat, m_lng);
        MarkerOptions markerOptions = new MarkerOptions();
        markerOptions.position(locat);
        map.addMarker(markerOptions);
        map.moveCamera(CameraUpdateFactory.newLatLng(locat));
        map.animateCamera(CameraUpdateFactory.zoomTo(15));
    }

    public void SetFragment(MapFragment Mapfragment)
    {
        m_mapFragment = Mapfragment;
    }
    public void SetID(int id)
    {

        m_mapFragment = (MapFragment)getFragmentManager().findFragmentById(id);

    }
    public void SetLatLng(Float lat,Float lng,MapFragment mapFragment)
    {
        m_lat = lat;
        m_lng = lng;
        m_mapFragment.getMapAsync(this);
    }

}