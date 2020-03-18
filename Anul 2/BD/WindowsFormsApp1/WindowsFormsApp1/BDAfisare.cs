using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class BDAfisare : Form
    {
        public BDAfisare()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            using (UserEntities context = new UserEntities())
            {
                try
                {
                    dataGridView1.DataSource = context.tblUser2.ToList();
                }catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

        }
    }
}
