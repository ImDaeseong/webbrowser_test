using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CefSharp;
using CefSharp.WinForms;
using System.Windows.Forms;

namespace WinFormsChromeBrowser2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            CefSettings settings = new CefSettings();
            Cef.Initialize(new CefSettings());
        }

        private void Navigate(string url)
        {
            if (!string.IsNullOrEmpty(url) && !url.StartsWith("http://") && !url.StartsWith("https://"))
            {
                url = "https://" + url;
            }
            chromiumWebBrowser1.Load(url);
        }

        private void GoBack()
        {
            if (chromiumWebBrowser1.CanGoBack)
                chromiumWebBrowser1.Back();
        }

        private void GoForward()
        {
            if (chromiumWebBrowser1.CanGoForward)
                chromiumWebBrowser1.Forward();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Navigate("https://www.naver.com");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Cef.Shutdown();

            if (chromiumWebBrowser1 != null)
            {
                chromiumWebBrowser1.Dispose();
            }
        }

        private void chromiumWebBrowser1_AddressChanged(object sender, AddressChangedEventArgs e)
        {
            string sMsg = string.Format("chromiumWebBrowser1_AddressChanged :{0} ", e.Address);
            Console.WriteLine(sMsg);
        }

        private void chromiumWebBrowser1_TitleChanged(object sender, TitleChangedEventArgs e)
        {
            string sMsg = string.Format("chromiumWebBrowser1_TitleChanged :{0} ", e.Title);
            Console.WriteLine(sMsg);
        }

        private void chromiumWebBrowser1_StatusMessage(object sender, StatusMessageEventArgs e)
        {
            string sMsg = string.Format("chromiumWebBrowser1_StatusMessage :{0} ", e.Value);
            Console.WriteLine(sMsg);
        }

        private void chromiumWebBrowser1_LoadError(object sender, LoadErrorEventArgs e)
        {
            string sMsg = string.Format("chromiumWebBrowser1_LoadError :{0} ", e.ErrorText);
            Console.WriteLine(sMsg);
        }

        private void chromiumWebBrowser1_LoadingStateChanged(object sender, LoadingStateChangedEventArgs e)
        {
            Console.WriteLine("e.IsLoading:" + e.IsLoading);
            
            string sMsg = (e.IsLoading ? "Loading..." : chromiumWebBrowser1.Address);
            Console.WriteLine(sMsg);

            if (e.IsLoading == false)
            {
                Console.WriteLine("브라우저 로드 완료");
            }
        }
       
    }
}
