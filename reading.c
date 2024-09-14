#include "pipex.h"

static char	*ft_free(char *str, char *buff)
{
	free(str);
	free(buff);
	return (NULL);
}

static char	*reader(int fd, char *buff, char *str, int chars_read)
{
	char	*s;

	while (chars_read != 0)
	{
		chars_read = read(fd, buff, BUFFER_SIZE);
		if (chars_read == -1)
			return (ft_free(s, buff));
		if (chars_read == 0)
		{
			free(buff);
			return (str);
		}
		buff[chars_read] = '\0';
		s = str;
		str = ft_strjoin(str, buff);
		if (!str)
			return (ft_free(s, buff));
		free(s);
	}
	free(buff);
	return (str);
}

static char *fetching_from_file(int fd)
{
    char *buff;
    char *content;

    content = NULL;
    if (fd == -1)
    {
        printf("Error in open()-ing\n");
        return (NULL);
    }

    buff = malloc((sizeof(char) * BUFFER_SIZE) + 1);
    if (!buff)
        printf("Error in malloc()-ing\n");
    content = reader(fd, buff, content, 1);
    if (!content)
        printf("Error in reading file\n");
    printf("From file: %s\n", content);
    return (content);
}