from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('problem', '0016_chapter'),
    ]

    operations = [
        migrations.CreateModel(
            name='ClassSession',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.TextField()),
                ('start_time', models.DateTimeField()),
                ('end_time', models.DateTimeField()),
                ('create_time', models.DateTimeField(auto_now_add=True)),
                ('chapter', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='sessions', to='problem.chapter')),
                ('created_by', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
            options={'db_table': 'class_session', 'ordering': ('-start_time',)},
        ),
        migrations.CreateModel(
            name='StudentActivity',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('ip_address', models.TextField(default='')),
                ('updated_at', models.DateTimeField(auto_now=True)),
                ('current_problem', models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.SET_NULL, to='problem.problem')),
                ('session', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='activities', to='problem.classsession')),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
            options={'db_table': 'student_activity'},
        ),
        migrations.AlterUniqueTogether(
            name='studentactivity',
            unique_together={('session', 'user')},
        ),
        migrations.CreateModel(
            name='IPBinding',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('ip_address', models.TextField(unique=True)),
                ('hostname', models.TextField()),
                ('note', models.TextField(blank=True, null=True)),
                ('updated_at', models.DateTimeField(auto_now=True)),
            ],
            options={'db_table': 'ip_binding'},
        ),
    ]
