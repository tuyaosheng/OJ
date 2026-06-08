from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('problem', '0015_problem_video'),
    ]

    operations = [
        migrations.CreateModel(
            name='Chapter',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.TextField()),
                ('description', models.TextField(blank=True, null=True)),
                ('order', models.IntegerField(default=0)),
                ('create_time', models.DateTimeField(auto_now_add=True)),
                ('created_by', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
            options={'db_table': 'chapter', 'ordering': ('order', 'create_time')},
        ),
        migrations.CreateModel(
            name='ChapterProblem',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('order', models.IntegerField(default=0)),
                ('chapter', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='chapter_problems', to='problem.chapter')),
                ('problem', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='problem.problem')),
            ],
            options={'db_table': 'chapter_problem', 'ordering': ('order',)},
        ),
        migrations.AddField(
            model_name='chapter',
            name='problems',
            field=models.ManyToManyField(blank=True, through='problem.ChapterProblem', to='problem.Problem'),
        ),
        migrations.AlterUniqueTogether(
            name='chapterproblem',
            unique_together={('chapter', 'problem')},
        ),
    ]
