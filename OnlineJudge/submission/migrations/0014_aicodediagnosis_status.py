from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('submission', '0013_aicodediagnosis'),
    ]

    operations = [
        migrations.AlterField(
            model_name='aicodediagnosis',
            name='result',
            field=models.TextField(blank=True, default=''),
        ),
        migrations.AddField(
            model_name='aicodediagnosis',
            name='status',
            field=models.CharField(db_index=True, default='success', max_length=16),
        ),
        migrations.AddField(
            model_name='aicodediagnosis',
            name='error',
            field=models.TextField(blank=True, null=True),
        ),
    ]
