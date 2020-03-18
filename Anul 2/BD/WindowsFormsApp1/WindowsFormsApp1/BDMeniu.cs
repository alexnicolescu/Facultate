using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;
namespace WindowsFormsApp1
{
    public partial class BDMeniu : Form
    {
        public BDMeniu()
        {
            InitializeComponent();
        }

       
        private void button1_Click(object sender, EventArgs e)
        {
            BDAdaugare f = new BDAdaugare();
            f.ShowDialog();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            BDEditare f = new BDEditare();
            f.ShowDialog();
        }
        private void button3_Click(object sender, EventArgs e)
        {
            BDStergere f = new BDStergere();
            f.ShowDialog();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            BDCautare f = new BDCautare();
            f.ShowDialog();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            BDAfisare f = new BDAfisare();
            f.ShowDialog();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }

}
