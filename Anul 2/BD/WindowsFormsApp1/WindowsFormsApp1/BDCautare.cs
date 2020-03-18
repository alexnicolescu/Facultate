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
    public partial class BDCautare : Form
    {
        public BDCautare()
        {
            InitializeComponent();
        }

        private void Cautare_Click(object sender, EventArgs e)
        {
            if (Equals(txtCNP.Text, "") || !OnlyNumbers() || txtCNP.Text.Length != 13)
                MessageBox.Show("Introduceti un CNP valid");
            else
            {
                using (UserEntities context = new UserEntities())
                {
                    try {
                        tblUser2 obj = context.tblUser2.FirstOrDefault(r => r.CNP == txtCNP.Text);
                        if (obj != null)
                        {
                            txtNume.Text = obj.Nume;
                            txtPrenume.Text = obj.Prenume;
                            txtAdresa.Text = obj.Adresa;
                        }
                        else
                            MessageBox.Show("Persoana cautata nu exista");
                    }catch(Exception ex)
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
