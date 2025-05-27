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
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.IO;

namespace WinFormsChromeBrowser2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            //CefSettings settings = new CefSettings();
            //Cef.Initialize(new CefSettings());

            InitializeCefSharp();
        }

        private void InitializeCefSharp()
        {
            var settings = new CefSettings
            {
                CachePath = Path.Combine(Path.GetTempPath(), "cef_cache_" + Guid.NewGuid()),
                LogSeverity = LogSeverity.Disable,
                RemoteDebuggingPort = 9222 // 디버깅용
            };

            // 필요한 경우 추가 설정 가능
            settings.CefCommandLineArgs.Add("disable-gpu", "1"); // GPU 비활성화
            settings.CefCommandLineArgs.Add("disable-gpu-compositing", "1");

            Cef.Initialize(settings);
        }

        private void Navigate(string url)
        {
            if (!string.IsNullOrEmpty(url) && !url.StartsWith("http://") && !url.StartsWith("https://"))
            {
                url = "https://" + url;
            }
            chromiumWebBrowser1.Load(url);
        }

        private void NavigateLoadHtml()
        {
            //경로에서 html 가져오기
            /*
            string sHtmlPath = string.Format("{0}\\webrtc.html", Application.StartupPath);
            chromiumWebBrowser1.LoadHtml(sHtmlPath);
            */

            //리소스에서 html 가져오기
            string sHtmlPath = Properties.Resources.webrtc;
            chromiumWebBrowser1.LoadHtml(sHtmlPath);
        }

        private void SendMessage(string sMessage)
        {
            //웹페이지로 메시지 전달
            string js = $"window.receiveFromForm({System.Text.Json.JsonSerializer.Serialize(sMessage)});";
            chromiumWebBrowser1.ExecuteScriptAsync(js);
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
            //NavigateLoadHtml();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (chromiumWebBrowser1 != null)
            {
                chromiumWebBrowser1.Dispose();
            }

            Cef.Shutdown();
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

        private void chromiumWebBrowser1_JavascriptMessageReceived(object sender, JavascriptMessageReceivedEventArgs e)
        {
            //웹페이지에서 전달받은 메시지
            string sMsg = e.Message.ToString();
            Console.WriteLine(sMsg);
        }
    }
}
