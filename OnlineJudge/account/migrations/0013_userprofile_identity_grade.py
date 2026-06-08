from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('account', '0012_userprofile_language'),
    ]

    operations = [
        migrations.AddField(
            model_name='userprofile',
            name='identity',
            field=models.TextField(default=''),
        ),
        migrations.AddField(
            model_name='userprofile',
            name='grade',
            field=models.IntegerField(blank=True, null=True),
        ),
        migrations.AddField(
            model_name='userprofile',
            name='class_name',
            field=models.TextField(blank=True, null=True),
        ),
    ]
