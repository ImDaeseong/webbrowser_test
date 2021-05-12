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

        private void Form1_Load(object sender, EventArgs e)
        {
            if (webView != null)
            {
                webView.Source = new Uri("https://www.naver.com");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (webView != null)
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
    }
}
