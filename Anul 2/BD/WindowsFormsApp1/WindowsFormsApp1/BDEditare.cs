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
    public partial class BDEditare : Form
    {
        //private String connectionString = @"Data Source=DESKTOP-2VBO84U\SQLEXPRESS;Initial Catalog=UserAdderDB;Integrated Security=True";
        public BDEditare()
        {
            InitializeComponent();
        } 
        private void Button1_Click(object sender, EventArgs e)
        {
            if (Equals(txtCNP.Text, "") || !OnlyNumbers() || txtCNP.Text.Length != 13)
                MessageBox.Show("Introduceti un CNP valid");
            else
                if (Equals(txtNume.Text.Trim(), "") && Equals(txtPrenume.Text.Trim(), "") && Equals(txtAdresa.Text.Trim(), ""))
                MessageBox.Show("Trebuie modificat cel putin un camp");
            else
            {
                using (UserEntities context = new UserEntities())
                {
                    try
                    {
                        tblUser2 obj = context.tblUser2.FirstOrDefault(r => r.CNP == txtCNP.Text);
                        if (obj != null)
                        {
                            if (!Equals(txtNume.Text.Trim(), ""))
                                obj.Nume = txtNume.Text.Trim();
                            if (!Equals(txtPrenume.Text.Trim(), ""))
                                obj.Prenume = txtPrenume.Text.Trim();
                            if (!Equals(txtAdresa.Text.Trim(), ""))
                                obj.Adresa = txtAdresa.Text.Trim();
                            context.SaveChanges();
                            MessageBox.Show("Editare realizata cu succes");
                            Clear();
                        }
                        else
                        {
                            MessageBox.Show("Persoana cautata nu exista");

                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);

                    }
                   
                }
            }
        }
        private void Clear()//Goleste camp-urile din form
        {
            txtNume.Text = txtPrenume.Text = txtCNP.Text = txtAdresa.Text = "";
        }
        private Boolean OnlyNumbers()//Verifica daca CNP-ul este format doar in cifre
        {
            for (int i = 0; i < txtCNP.Text.Length; i++)
                if (txtCNP.Text[i] < '0' || txtCNP.Text[i] > '9')
                    return false;
            return true;
        }
    }
}
