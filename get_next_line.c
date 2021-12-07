#include "get_next_line.h"

int	ft_strlen(char	*str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int	i;
	int	j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = 0;
	return (str);
}

char	*ft_strdup(char *str)
{
	int	i;
	char	*cpy;

	i = 0;
	if (!str)
		return (NULL);
	cpy = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!cpy)
		return (NULL);
	while (str[i])
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

char	*ft_strchr(char *str, char c)
{
	char	*ret;

	if (!str)
		return (NULL);
	ret = str;
	while (*ret != c)
	{
		if (*ret == 0)
			return (NULL);
		ret++;
	}
	return (ret);
}

char	*get_line(char *tmp)
{
	int	i;
	char	*line;

	i = 0;
	// Choper la taille de tmp et rajouter une place si il y a un \n
	while (tmp[i] && tmp[i] != '\n')
		i++;
	if (tmp[i] == '\n')
		i++;
	// malloc de line avec i qui comprend deja le \n plus 1 avec le \0
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	// Remplissage de line
	i = 0;
	while (tmp[i] && tmp[i] != '\n')
	{
		line[i] = tmp[i];
		i++;
	}
	// Ajout du \n si il y en un
	if (tmp[i] == '\n')
	{
		line[i] = tmp[i];
		i++;
	}
	// ajout du \0
	line[i] = 0;
	return (line);
}

char	*get_tmp(char *tmp)
{
	int	i;
	int	j;
	char	*str;

	i = 0;
	// choper la taille de ce quil y a avant le \n
	while (tmp[i] && tmp[i] != '\n')
		i++;
	if (tmp[i] == '\n')
		i++;
	// Malloc de keta_strlen(tmp) - i + 1 avec le \0
	str = malloc(sizeof(char) * (ft_strlen(tmp) - i + 1));
	if (!str)
		return (NULL);
	// Renplissage de str[j++] a partir de i qui est deja a la bonne place dans tmp
	j = 0;
	while (tmp[i])
		str[j++] = tmp[i++];
	// le petit \0 qui fait plaizz
	str[j] = 0;
	return (str);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*cpy;
	static char	*tmp = NULL;
	char		buf[BUFFER_SIZE + 1];
	int		ret;

	ret = 1;
	line = NULL;
	if (fd < 0)
		return (NULL);
	// Remplissage de tmp jusqu'au \n
	while(!ft_strchr(tmp, '\n') && ret)
	{
		// Partie pour avoir ret
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == -1)
			return (NULL);
		buf[ret] = 0;
		// Mettre dans cpy la version actuelle de tmp, soit un chaine vide soit tmp
		if (!tmp)
			cpy = ft_strdup("");
		else
		{
			cpy = ft_strdup(tmp);
			free(tmp);
		}
		if (!cpy)
			return (NULL);
		// Mettre dans tmp un join de cpy et du buf actuel puis free cpy
		tmp = ft_strjoin(cpy, buf);
		free(cpy);
	}
	// Check si la fonction est fini, on le sait deja si ret == 0
	// et que tmp est vide (tmp[0] == \0)
	if (!ret && !tmp[0])
	{
		free(tmp);
		return (NULL);
	}
	// Pareil que pour la loop qui rempli tmp,
	// on met la version actuelle de tmp dans cpy puis on free tmp
	cpy = ft_strdup(tmp);
	if (!cpy)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	// on met dans line tout ce quil y a avant le \n (voir fonction get_line)
	line = get_line(cpy);
	if (!line)
	{
		free(cpy);
		return (NULL);
	}
	// Puis on met tout ce quil y a apres dans tmp (voir get_tmp)
	tmp = get_tmp(cpy);
	if (!tmp)
	{
		free(cpy);
		free(line);
		return (NULL);
	}
	free(cpy);
	// On return simplement line puisquon a deja checke si le fichier
	// etait fini avant dessayer de remplir quoi que ce soit
	return (line);
}

int	main(int ac, char **av)
{
	int	fd;
	char	*line;

	if (ac != 2)
		return (-1);
	fd = open(av[1], O_RDONLY);
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	return (-1);
}
