using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsWebView2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Navigate(string sUrl)
        {
            if (!string.IsNullOrEmpty(sUrl) && !sUrl.StartsWith("http://") && !sUrl.StartsWith("https://"))
            {
                sUrl = "https://" + sUrl;
            }
            webView.CoreWebView2.Navigate(sUrl);
        }

        private void GoBack()
        {
            if (webView.CoreWebView2.CanGoBack)
                webView.CoreWebView2.GoBack();
        }

        private void GoForward()
        {
            if (webView.CoreWebView2.CanGoForward)
                webView.CoreWebView2.GoForward();
        }

        private async void Form1_Load(object sender, EventArgs e)
        {
            //WebView2 런타임 초기화
            await webView.EnsureCoreWebView2Async(null);

            Navigate("https://www.naver.com");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (webView != null && !webView.IsDisposed)
            {
                webView.Dispose();
            }
        }

        private void webView_NavigationStarting(object sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationStartingEventArgs e)
        {
            Console.WriteLine("webView_NavigationStarting");
            Console.WriteLine(e.Uri);
        }

        private void webView_NavigationCompleted(object sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationCompletedEventArgs e)
        {
            Console.WriteLine("webView_NavigationCompleted");
            string surl = this.webView.Source.ToString();
            Console.WriteLine(surl);
        }

        private void webView_SourceChanged(object sender, Microsoft.Web.WebView2.Core.CoreWebView2SourceChangedEventArgs e)
        {
            Console.WriteLine("webView_SourceChanged");
            string surl = webView.Source.ToString();
            Console.WriteLine(surl);
        }
    }
}
