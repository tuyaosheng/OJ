from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('problem', '0017_class_session'),
        ('submission', '0012_auto_20180501_0436'),
    ]

    operations = [
        migrations.CreateModel(
            name='AICodeDiagnosis',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('user_id', models.IntegerField(db_index=True)),
                ('username', models.TextField()),
                ('submission_result', models.IntegerField(default=-1)),
                ('result', models.TextField()),
                ('create_time', models.DateTimeField(auto_now_add=True)),
                ('problem', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='problem.problem')),
                ('submission', models.OneToOneField(on_delete=django.db.models.deletion.CASCADE, related_name='ai_diagnosis', to='submission.submission')),
            ],
            options={
                'db_table': 'ai_code_diagnosis',
                'ordering': ('-create_time',),
            },
        ),
    ]
