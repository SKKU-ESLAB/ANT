package com.ant.ant_manager.controller;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Dongig Sin<dongig@skku.edu>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.util.Log;


//Make new json string
/*
 * 	LegacyJSONParser jp2 = new LegacyJSONParser();
	jp2.makeNewJson();

	jp2.addJsonKeyValue("key1", "value1");
	jp2.addJsonKeyValue("key2", "value2");

	Log.d("ANT", jp2.getJsonData());
*/



//Use existing json to parse
/*
 * 	LegacyJSONParser jp = new LegacyJSONParser("{\"noti\":\"_appid\",\"appTitle\":\"noti example\",\"dateTime\":\"2015-08-15, 11:58 am\",\"description\":\"ANTEvent detected\",\"text\":\"eventDetected\",\"img\":\"mFilePath.jpg\"}");

	Log.d("ANT", jp.getValueByKey("noti"));
	Log.d("ANT", jp.getValueByKey("img"));

	while(jp.hasMoreValue()){
			String ret[] = new String[2];
			ret=jp.getNextKeyValue();

			Log.d("ANT", ret[0] + " : " + ret[1] );
	}
*/

public class LegacyJSONParser {
	
	private String jsonData;
	
	public LegacyJSONParser(){
		jsonData="";
	}
	
	public LegacyJSONParser(String str){
		jsonData=str;
	}
	
	public String getJsonData(){
		return jsonData;
	}
	
	
	public void makeNewJson(){
		
		jsonData = "{}";
	}
	
	public void addJsonKeyValue(String key, String value){
		
		jsonData = jsonData.substring(0, jsonData.length()-1);
		if(jsonData.length() == 1){
			jsonData = jsonData + "\"" + key + "\":\""+value+"\"}";
		}
		else{
			jsonData = jsonData + ",\"" + key + "\":\""+value+"\"}";	
		}
	}
	
	public boolean hasMoreValue(){
		
		if(jsonData.equals("") || jsonData.isEmpty() || jsonData == null)
			return false;
		
		return true;
	}
	
	public String[] getNextKeyValue(){
		int position = 0;
		String[] keyValue ={"", ""};
		
		for(int i=0; i<jsonData.length(); i++){

			if ( jsonData.charAt(i) == '{' )
				continue;

			else if ( jsonData.charAt(i) == '}' ){
				jsonData="";
			}
			
			else if ( jsonData.charAt(i) == ':' ){
				if( jsonData.charAt(i+1) == '"'){
					position++;
				}
				else{
					if( (position % 2) == 0 ) // even num -> key
						keyValue[0] += jsonData.charAt(i);
					else				// odd num -> value
						keyValue[1] +=jsonData.charAt(i);
				}
			}

			else if ( jsonData.charAt(i) == ',' ){
				
				if( jsonData.charAt(i+1) == '"' ){
					position++;
					jsonData = jsonData.substring(i+1, jsonData.length());

					return keyValue;
				}
				else if(jsonData.charAt(i+1) == '}'){
					position++;
					jsonData = "";
					return keyValue;
				}
			}

			else if ( jsonData.charAt(i) == '"' ){
				continue;
			}

			else{

				if( (position % 2) == 0 ) // even num -> key
					keyValue[0] += jsonData.charAt(i);
				else				// odd num -> value
					keyValue[1] +=jsonData.charAt(i);
			}
		}
	
		return keyValue;
	}
	
	public String getValueByKey(String name){
		int position = 0;
		String key="";
		String value="";
				
		for(int i=0; i<jsonData.length(); i++){

			if ( jsonData.charAt(i) == '{' )
				continue;

			else if ( jsonData.charAt(i) == '}' ){
				if(key.equals(name)){
					
					return value;
				}
			}
			
			else if ( jsonData.charAt(i) == ':' ){
				if( jsonData.charAt(i+1) == '"')
					position++;
			}

			else if ( jsonData.charAt(i) == ',' ){
				if( jsonData.charAt(i+1) == '"'){
					position++;

					if(key.equals(name)){
						
						return value;
					}
					else{
						key="";
						value="";
					}
				}
				else{
					if( (position % 2)==0 ) // even num -> key
						key+= jsonData.charAt(i);
					else				// odd num -> value
						value+=jsonData.charAt(i);
				}
			}

			else if ( jsonData.charAt(i) == '"' ){
				continue;
			}

			else{

				if( (position % 2)==0 ) // even num -> key
					key+= jsonData.charAt(i);
				else				// odd num -> value
					value+=jsonData.charAt(i);
			}
		}
		Log.d("ANT", "CANNOT fine the value by key : key-" + name);
		return "";
	}
}