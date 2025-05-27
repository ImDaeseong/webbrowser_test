using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.Policy;
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

        private void NavigateToString()
        {
            //경로에서 html 가져오기
            /*
            string sHtmlPath = string.Format("{0}\\webrtc.html", Application.StartupPath);
            webView.CoreWebView2.Navigate(sHtmlPath);
            */

            //리소스에서 html 가져오기
            string sHtmlPath = Properties.Resources.webrtc;
            webView.CoreWebView2.NavigateToString(sHtmlPath);
        }

        private void SendMessage(string sMessage)
        {
            //웹페이지로 메시지 전달
            string js = $"window.receiveFromForm({System.Text.Json.JsonSerializer.Serialize(sMessage)});";
            webView.CoreWebView2.ExecuteScriptAsync(js);
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

            //NavigateToString();
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

        private void webView_WebMessageReceived(object sender, Microsoft.Web.WebView2.Core.CoreWebView2WebMessageReceivedEventArgs e)
        {
            //웹페이지에서 전달받은 메시지
            string sMessage = e.TryGetWebMessageAsString();
            Console.WriteLine(sMessage);
        }
    }
}
