using Newtonsoft.Json;
using RestSharp;
using System;
using System.Collections;
using System.IO;
using System.Text;
using System.Windows.Forms;
using GMap;
using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System.Threading;

namespace Form1
{
    public partial class Form1 : Form
    {
       
        static string[] lines = File.ReadAllLines("C:/Users/asli.zoumpoul/Desktop/pastemails.txt");
        int k = 0;
        string[] IP = new string[lines.Length];
        string[] EmailAddress = new string[lines.Length];
        string[] Subject = new string[lines.Length];
        int i = 0;
        string[] City = new string[lines.Length];
        string[] Latitude = new string[lines.Length];
        string[] Longitude = new string[lines.Length];
        string[] Country = new string[lines.Length];
        string [] charsToRemove = new string[] { "\"", "]", "[" };
        
        public Form1()
        {
            InitializeComponent();
            try
            {
                foreach (string line in lines)
                {
                    string[] arr = line.Split(',');

                    foreach (string c in charsToRemove)
                    {   
                        
                            arr[0] = arr[0].Replace(c, "");
                            arr[1] = arr[1].Replace(c, "");
                            arr[2] = arr[2].Replace(c, "");
                        
                    }
                    if (arr[0].Equals("") == false)
                    {
                        IP[i] = arr[0];
                        EmailAddress[i] = arr[1];
                        Subject[i] = arr[2];

                    }
                    else
                    {
                        IP[i] = "Empty";
                        EmailAddress[i] = "Empty";
                        Subject[i] = "Empty";
                    }
                    i++;


                }
             
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), "Error");
            }
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            
        }
        private void BtnFetchCurrentIPLocation_Click(object sender, EventArgs e)
        {
            txtIPLocationResult.Text=FetchCurrentIPLocation();
        }

        public void BtnFetchLocationDetails_Click(object sender, EventArgs e)
        {
            
            try
            {
                while (k < IP.Length)
                {
                    if (IP[k].Equals("Empty") == false)
                    {
                        txtIPAddress.Text = IP[k].ToString();
                        txtIPLocationResult.Text = FetchCurrentIPLocation(txtIPAddress.Text);
                        if (txtIPLocationResult.Lines[12].Equals("city: ") == false)
                        {
                            City[k] = txtIPLocationResult.Lines[12].Remove(0, 6);
                            Latitude[k] = txtIPLocationResult.Lines[13].Remove(0, 10);
                            Longitude[k] = txtIPLocationResult.Lines[14].Remove(0, 11);
                            Country[k] = txtIPLocationResult.Lines[5].Remove(0, 9);

                       }
                        else
                       {
                            City[k] = "Empty";
                            Latitude[k] = "Empty";
                            Longitude[k] = "Empty";
                            Country[k] = "Empty";
                        }
                       
                    }
                    else
                    {
                        City[k] = "Empty";
                        Latitude[k] = "Empty";
                        Longitude[k] = "Empty";
                        Country[k] = "Empty";
                    }
                    k++;

                }
                gmap.MapProvider = GMap.NET.MapProviders.BingMapProvider.Instance;
                GMap.NET.GMaps.Instance.Mode = GMap.NET.AccessMode.ServerOnly;

               
                    //Thread t = new Thread(WriteY);          // Kick off a new thread
                    //t.Start();                               // running WriteY()

                

               
                for (int l = 0; l < IP.Length/2; l++)
                {
                    if (City[l].Equals("Empty")==false)
                    {
                        gmap.SetPositionByKeywords(City[l]);
                        gmap.Position = new GMap.NET.PointLatLng(Convert.ToDouble(Latitude[l]), Convert.ToDouble(Longitude[l]));
                        gmap.ShowCenter = false;
                        GMap.NET.WindowsForms.GMapOverlay markers = new GMap.NET.WindowsForms.GMapOverlay("markers");
                        GMap.NET.WindowsForms.GMapMarker marker =
                            new GMap.NET.WindowsForms.Markers.GMarkerGoogle(
                                new GMap.NET.PointLatLng(Convert.ToDouble(Latitude[l]), Convert.ToDouble(Longitude[l])),
                                GMap.NET.WindowsForms.Markers.GMarkerGoogleType.blue_pushpin);
                        markers.Markers.Add(marker);
                        gmap.Overlays.Add(markers);
                    //    marker.ToolTipMode = MarkerTooltipMode.Always;
                    //    marker.ToolTipText = EmailAddress[l] + "\n" + City[l] + "\n" + Country[l];
                    }
                }
                txtIPAddress.Clear();
                txtIPLocationResult.Text = "Locations are shown";
               


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message.ToString(), "Error");
            }
            
        }

        //public void WriteY()
        //{
        //    for (int l = IP.Length / 2; l < IP.Length ; l++)
        //    {
        //        if (City[l].Equals("Empty") == false)
        //        {
        //            gmap.SetPositionByKeywords(City[l]);
        //            gmap.Position = new GMap.NET.PointLatLng(Convert.ToDouble(Latitude[l]), Convert.ToDouble(Longitude[l]));
        //            gmap.ShowCenter = false;
        //            GMap.NET.WindowsForms.GMapOverlay markers = new GMap.NET.WindowsForms.GMapOverlay("markers");
        //            GMap.NET.WindowsForms.GMapMarker marker =
        //                new GMap.NET.WindowsForms.Markers.GMarkerGoogle(
        //                    new GMap.NET.PointLatLng(Convert.ToDouble(Latitude[l]), Convert.ToDouble(Longitude[l])),
        //                    GMap.NET.WindowsForms.Markers.GMarkerGoogleType.blue_pushpin);
        //            markers.Markers.Add(marker);
        //            gmap.Overlays.Add(markers);
        //            marker.ToolTipMode = MarkerTooltipMode.Always;
        //            marker.ToolTipText = EmailAddress[l] + "\n" + City[l] + "\n" + Country[l];
        //        }
        //    }
        //    txtIPAddress.Clear();
        //    txtIPLocationResult.Text = "Locations are shown";
        //}

        private string FetchCurrentIPLocation()
        {
            
                string strIpLocation = string.Empty;
                var client = new RestClient("http://free.ipwhois.io/json/217.131.28.247");
                var request = new RestRequest()
                {
                    Method = Method.GET

                };
                var response = client.Execute(request);

                var dictionary = JsonConvert.DeserializeObject<IDictionary>(response.Content);
                foreach (var key in dictionary.Keys)
                {
                    strIpLocation += key.ToString() + ": " + dictionary[key] + "\r\n";
                }
                return strIpLocation;
            
            
            
        }
        
        private string FetchCurrentIPLocation(string strIP)
        {
            string strIpLocation = string.Empty;
            var client = new RestClient("http://free.ipwhois.io/json/" + strIP );
            var request = new RestRequest()
            {
                Method = Method.GET

            };
            var response = client.Execute(request);

            var dictionary = JsonConvert.DeserializeObject<IDictionary>(response.Content);
            foreach (var key in dictionary.Keys)
            {
                strIpLocation += key.ToString() + ": " + dictionary[key] + "\r\n";
            }
            return strIpLocation;
           
        }
        private void GMapControl1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
        }
    }
   
}
